#include "pricing.hpp"
#include "rng.hpp"
#include "params.hpp"
#include <algorithm>
#include <cmath>

Result european(const Params& p) {
    auto drift = (p.r - p.sigma * p.sigma / 2) * p.T;
    auto vol = p.sigma * std::sqrt(p.T);
    auto sum = 0.0;
    auto sum_sq = 0.0;
    for (int i{}; i < p.paths; ++i) {
        auto Z = normal(p.seed, i, 0);
        auto SI = p.S0 * std::exp(drift + vol * Z);
        auto payoff = std::max(p.K - SI, 0.0);
        sum += payoff;
        sum_sq += payoff * payoff;
    }

    auto mean = sum / p.paths;
    auto price = std::exp(-p.r * p.T) * mean;
    auto error = std::exp(-p.r * p.T) * std::sqrt((sum_sq / p.paths - mean * mean) / p.paths);
    return {price, error};
}
