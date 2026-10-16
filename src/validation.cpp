#include "validation.hpp"
#include "functions.hpp"
#include <cmath>
#include <vector>

double normal_cdf(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2));
}

double black_scholes_call(const Option& option, const MarketData& market, const SimulationData&) {
    double d1 = (std::log(option.S0 / option.K) + (market.r + 0.5 * market.sigma * market.sigma) * option.T) / (market.sigma * std::sqrt(option.T));
    double d2 = d1 - market.sigma * std::sqrt(option.T);
    return option.S0 * normal_cdf(d1) - option.K * std::exp(-market.r * option.T) * normal_cdf(d2);
}

double black_scholes_put(const Option& option, const MarketData& market, const SimulationData&) {
    double d1 = (std::log(option.S0 / option.K) + (market.r + 0.5 * market.sigma * market.sigma) * option.T) / (market.sigma * std::sqrt(option.T));
    double d2 = d1 - market.sigma * std::sqrt(option.T);
    return option.K * std::exp(-market.r * option.T) * normal_cdf(-d2) - option.S0 * normal_cdf(-d1);
}
