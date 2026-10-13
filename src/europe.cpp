#include "params.hpp"
#include "result.hpp"
#include "normal.hpp"
#include "functions.hpp"

Result european(const Params& params) {
    const auto dt = params.T / params.steps;
    const auto drift = (params.r - 0.5 * params.sigma * params.sigma) * dt;
    const auto volatility = params.sigma * std::sqrt(dt);

    auto sum{0.0};
    auto sum_sq{0.0};

    for (long path = 0; path < params.paths; ++path) {
        auto S = params.S0;
        for (long step = 0; step < params.steps; ++step) {
            double Z = normal(params.seed, path, step);
            S *= std::exp(drift + volatility * Z);
        }
        const auto payoff = std::max(params.K - S, 0.0);
        sum += payoff;
        sum_sq += payoff * payoff;
    }

    const auto mean = sum / params.paths;
    const auto price = std::exp(-params.r * params.T) * mean;
    const auto error = std::exp(-params.r * params.T) * std::sqrt((sum_sq / params.paths - mean * mean) / (params.paths));

    return {price, error};
}