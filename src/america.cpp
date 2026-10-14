#include "functions.hpp"
#include <vector>
#include <Eigen/Dense>
#include <thread>
#include <barrier>

Result american(const Params& params) {
    const auto dt = params.T / params.steps;
    const auto drift = (params.r - 0.5 * params.sigma * params.sigma) * dt;
    const auto volatility = params.sigma * std::sqrt(dt);
    const auto disc = std::exp(-params.r * dt);

    size_t N{params.paths};
    size_t M{params.steps};

    // If step * paths + path all paths at the same step on cache
    std::vector<double> S(N * M);

    for (size_t path = 0; path < N; ++path) {
        auto s = params.S0;
        for (size_t step = 0; step < M; ++step) {
            double Z = normal(params.seed, path, step);
            s *= std::exp(drift + volatility * Z);
            S[step * N + path] = s;
        }
    }

    std::vector<double> cash(N);
    for (size_t path = 0; path < N; ++path) {
        cash[path] = std::max(params.K - S[(M - 1) * N + path], 0.0);
    }

    for (int step = static_cast<int>(M) - 2; step >= 0; --step) {
        const double* S_step = &S[step * N];
        Eigen::Matrix3d A = Eigen::Matrix3d::Zero();
        Eigen::Vector3d b = Eigen::Vector3d::Zero();
        for (size_t path = 0; path < N; ++path) {
            cash[path] *= disc;
            auto s = S_step[path];
            if (s < params.K) {
                s = s / params.K;
                A(0, 0)++;
                A(0, 1) = A(1, 0) = (A(0, 1) + s); 
                A(0, 2) = A(2, 0) = A(1, 1) = (A(0, 2) + s * s);
                A(1, 2) = A(2, 1) = (A(1, 2) + s * s * s);
                A(2, 2) = (A(2, 2) + s * s * s * s);
                b(0) += cash[path];
                b(1) += cash[path] * s;
                b(2) += cash[path] * s * s;
            }
        }
        if (A(0, 0) < 3) {
            continue;
        }

        const Eigen::Vector3d beta = A.ldlt().solve(b);
        for (size_t path = 0; path < N; ++path) {
            auto s = S_step[path];
            if (s < params.K) {
                s = s / params.K;
                double continuation_value = beta(0) + beta(1) * s + beta(2) * s * s;
                double exercise_value = params.K - S_step[path];
                if (exercise_value > continuation_value) {
                    cash[path] = exercise_value;
                }
            }
        }
    }

    auto sum{0.0};
    auto sum_sq{0.0};

    for (long path = 0; path < N; ++path) {
        auto payoff = cash[path] * disc;
        sum += payoff;
        sum_sq += payoff * payoff;
    }

    const auto mean = sum / N;
    const auto price = std::exp(-params.r * params.T) * mean;
    const auto error = std::sqrt((sum_sq / N - mean * mean) / (N));

    return {std::max(mean, params.K - params.S0), error};
}

Result american_threads(const Params& params, int num_threads) {
    const auto dt = params.T / params.steps;
    const auto drift = (params.r - 0.5 * params.sigma * params.sigma) * dt;
    const auto volatility = params.sigma * std::sqrt(dt);
    const auto disc = std::exp(-params.r * dt);

    size_t N{params.paths};
    size_t M{params.steps};

    // If step * paths + path all paths at the same step on cache
    std::vector<double> sums(num_threads, 0.0);
    std::vector<double> sums_sq(num_threads, 0.0);
    std::vector<std::array<std::array<double, 3>, 3>> XtX(num_threads, {{0.0}});
    std::vector<std::array<double, 3>> XtY(num_threads, {0.0});
    Eigen::Matrix3d A = Eigen::Matrix3d::Zero();
    Eigen::Vector3d b = Eigen::Vector3d::Zero();
    std::vector<double> S(N * M);
    std::vector<double> cash(N);
    std::vector<std::thread> threads{};
    std::barrier sync_point(num_threads);
    for (int thread_id{}; thread_id < num_threads; ++thread_id) {
        threads.emplace_back([&, thread_id]() {
            long start = (N * thread_id) / num_threads;
            long end = (N * (thread_id + 1)) / num_threads;
            std::array<std::array<double, 3>, 3> XtX_thread{{0.0}};
            std::array<double, 3> XtY_thread{0.0};
            for (size_t path = start; path < end; ++path) {
                auto s = params.S0;
                for (size_t step = 0; step < M; ++step) {
                    double Z = normal(params.seed, path, step);
                    s *= std::exp(drift + volatility * Z);
                    S[step * N + path] = s;
                    cash[path] = std::max(params.K - s, 0.0);
                }
            }
            
            sync_point.arrive_and_wait();
            
            for (int step = static_cast<int>(M) - 2; step >= 0; --step) {
                const double* S_step = &S[step * N];
                XtX_thread = {{0.0}};
                XtY_thread = {0.0};
                if (thread_id == 0) {
                A.setZero();
                }
                if (thread_id == num_threads - 1) {
                    b.setZero();
                }
                for (size_t path = start; path < end; ++path) {
                    cash[path] *= disc;
                    auto s = S_step[path];
                    if (s < params.K) {
                        s = s / params.K;
                        XtX_thread[0][0]++;
                        XtX_thread[0][1] += s;
                        XtX_thread[0][2] += s * s;
                        XtX_thread[1][0] += s;
                        XtX_thread[1][1] += s * s;
                        XtX_thread[1][2] += s * s * s;
                        XtX_thread[2][0] += s * s;
                        XtX_thread[2][1] += s * s * s;
                        XtX_thread[2][2] += s * s * s * s;
                        XtY_thread[0] += cash[path];
                        XtY_thread[1] += cash[path] * s;
                        XtY_thread[2] += cash[path] * s * s;
                    }
                }
                sync_point.arrive_and_wait();   
                if (thread_id == 0) {
                    for (int i = 0; i < num_threads; ++i) {
                        A(0, 0) += XtX_thread[0][0];
                        A(0, 1) += XtX_thread[0][1];
                        A(0, 2) += XtX_thread[0][2];
                        A(1, 0) += XtX_thread[1][0];
                        A(1, 1) += XtX_thread[1][1];
                        A(1, 2) += XtX_thread[1][2];
                        A(2, 0) += XtX_thread[2][0];
                        A(2, 1) += XtX_thread[2][1];
                        A(2, 2) += XtX_thread[2][2];

                        b(0) += XtY_thread[0];
                        b(1) += XtY_thread[1];
                        b(2) += XtY_thread[2];
                    }
                }
                sync_point.arrive_and_wait();
                if (A(0, 0) < 3) {
                    continue;
                }
                const Eigen::Vector3d beta = A.ldlt().solve(b);
                for (size_t path = start; path < end; ++path) {
                    auto s = S_step[path];
                    if (s < params.K) {
                        s = s / params.K;
                        double continuation_value = beta(0) + beta(1) * s + beta(2) * s * s;
                        double exercise_value = params.K - S_step[path];
                        if (exercise_value > continuation_value) {
                            cash[path] = exercise_value;
                        }
                    }
                }
                sync_point.arrive_and_wait();
                auto sum{0.0};
                auto sum_sq{0.0};
                for (long path = start; path < end; ++path) {
                    auto payoff = cash[path] * disc;
                    sum += payoff;
                    sum_sq += payoff * payoff;
                }
                sums[thread_id] = sum;
                sums_sq[thread_id] = sum_sq;
            }
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }


    auto sum{0.0};
    auto sum_sq{0.0};

    for (long path = 0; path < num_threads; ++path) {
        sum += sums[path];
        sum_sq += sums_sq[path];
    }

    const auto mean = sum / N;
    const auto price = std::exp(-params.r * params.T) * mean;
    const auto error = std::sqrt((sum_sq / N - mean * mean) / (N));

    return {std::max(mean, params.K - params.S0), error};
}