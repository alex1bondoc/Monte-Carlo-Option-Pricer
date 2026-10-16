#include "params.hpp"
#include "normal.hpp"
#include "functions.hpp"
#include <vector>
#include <thread>
#include <numeric>

Result european(const Option& option, const MarketData& market, const SimulationData& simulation) {
    const auto dt = option.T / simulation.steps;
    const auto drift = (market.r - 0.5 * market.sigma * market.sigma) * dt;
    const auto diffusion = market.sigma * std::sqrt(dt);

    // sum of payoffs
    auto sum{0.0};
    auto sum_sq{0.0};


    // simulate N trajectories
    for (long path = 0; path < simulation.paths; ++path) {
        auto S = market.S0;
        // look calculate the price every day
        for (long step = 0; step < simulation.steps; ++step) {
            double Z = normal(simulation.seed, path, step);
            S *= std::exp(drift + diffusion * Z);
        }
        // calculate the payoff for that day in a trajectory
        const auto payoff_ = option.payoff(S);
        // total sum of payoffs
        sum += payoff_;
        sum_sq += payoff_ * payoff_;
    }
    // average payoff
    const auto mean = sum / simulation.paths;
    // what should be the price of the option
    const auto price = std::exp(-market.r * option.T) * mean;
    const auto error = std::exp(-market.r * option.T) * std::sqrt((sum_sq / simulation.paths - mean * mean) / (simulation.paths));

    return {price, error};
}

Result european_threads(const Option& option, const MarketData& market, const SimulationData& simulation, int num_threads) {
    const auto dt = option.T / simulation.steps;
    const auto drift = (market.r - 0.5 * market.sigma * market.sigma) * dt;
    const auto diffusion = market.sigma * std::sqrt(dt);

    std::vector<double> sums(num_threads, 0.0);
    std::vector<double> sums_sq(num_threads, 0.0);
    std::vector<std::thread> threads{};
    for (int thread_id{}; thread_id < num_threads; ++thread_id) {
        threads.emplace_back([&sums, &sums_sq, &option, &market, &simulation, &drift, &diffusion, num_threads, thread_id]() {
            long start = (simulation.paths * thread_id) / num_threads;
            long end = (simulation.paths * (thread_id + 1)) / num_threads;
            double sum{};
            double sum_sq{};
            for (long path = start; path < end; ++path) {
                auto S = market.S0;
                for (long step = 0; step < simulation.steps; ++step) {
                    double Z = normal(simulation.seed, path, step);
                    S *= std::exp(drift + diffusion * Z);
                }
                const auto payoff_ = option.payoff(S);
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

    const auto mean = sum / simulation.paths;
    const auto price = std::exp(-market.r * option.T) * mean;
    const auto error = std::exp(-market.r * option.T) * std::sqrt((sum_sq / simulation.paths - mean * mean) / (simulation.paths));

    return {price, error};
}
