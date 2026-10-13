#include "functions.hpp"
#include <vector>
#include <Eigen/Dense>

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