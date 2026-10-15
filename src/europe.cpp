#include "params.hpp"
#include "result.hpp"
#include "normal.hpp"
#include "functions.hpp"
#include <vector>
#include <thread>
#include <numeric>

Result european(const Params& params) {
    const auto dt = params.T / params.steps;
    const auto drift = (params.r - 0.5 * params.sigma * params.sigma) * dt;
    const auto diffusion = params.sigma * std::sqrt(dt);

    auto sum{0.0};
    auto sum_sq{0.0};

    for (long path = 0; path < params.paths; ++path) {
        auto S = params.S0;
        for (long step = 0; step < params.steps; ++step) {
            double Z = normal(params.seed, path, step);
            S *= std::exp(drift + diffusion * Z);
        }
        const auto payoff_ = payoff(params, S);
        sum += payoff_;
        sum_sq += payoff_ * payoff_;
    }

    const auto mean = sum / params.paths;
    const auto price = std::exp(-params.r * params.T) * mean;
    const auto error = std::exp(-params.r * params.T) * std::sqrt((sum_sq / params.paths - mean * mean) / (params.paths));

    return {price, error};
}

Result european_threads(const Params& params, int num_threads) {
    const auto dt = params.T / params.steps;
    const auto drift = (params.r - 0.5 * params.sigma * params.sigma) * dt;
    const auto diffusion = params.sigma * std::sqrt(dt);

    std::vector<double> sums(num_threads, 0.0);
    std::vector<double> sums_sq(num_threads, 0.0);
    std::vector<std::thread> threads{};
    for (int thread_id{}; thread_id < num_threads; ++thread_id) {
        threads.emplace_back([&sums, &sums_sq, &params, &drift, &diffusion, num_threads, thread_id]() {
            long start = (params.paths * thread_id) / num_threads;
            long end = (params.paths * (thread_id + 1)) / num_threads;
            double sum{};
            double sum_sq{};
            for (long path = start; path < end; ++path) {
                auto S = params.S0;
                for (long step = 0; step < params.steps; ++step) {
                    double Z = normal(params.seed, path, step);
                    S *= std::exp(drift + diffusion * Z);
                }
                const auto payoff_ = payoff(params, S);
                sum += payoff_;
                sum_sq += payoff_ * payoff_;
            }
            sums[thread_id] = sum;
            sums_sq[thread_id] = sum_sq;
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }
    auto sum = std::accumulate(sums.begin(), sums.end(), 0.0);
    auto sum_sq = std::accumulate(sums_sq.begin(), sums_sq.end(), 0.0);

    const auto mean = sum / params.paths;
    const auto price = std::exp(-params.r * params.T) * mean;
    const auto error = std::exp(-params.r * params.T) * std::sqrt((sum_sq / params.paths - mean * mean) / (params.paths));

    return {price, error};
}