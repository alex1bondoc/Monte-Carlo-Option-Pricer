#include "black_scholes.hpp"
#include <cmath>

double normal_cdf(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2));
}

double black_scholes_call(const Params& params) {
    double d1 = (std::log(params.S0 / params.K) + (params.r + 0.5 * params.sigma * params.sigma) * params.T) / (params.sigma * std::sqrt(params.T));
    double d2 = d1 - params.sigma * std::sqrt(params.T);
    return params.S0 * normal_cdf(d1) - params.K * std::exp(-params.r * params.T) * normal_cdf(d2);
}

double black_scholes_put(const Params& params) {
    double d1 = (std::log(params.S0 / params.K) + (params.r + 0.5 * params.sigma * params.sigma) * params.T) / (params.sigma * std::sqrt(params.T));
    double d2 = d1 - params.sigma * std::sqrt(params.T);
    return params.K * std::exp(-params.r * params.T) * normal_cdf(-d2) - params.S0 * normal_cdf(-d1);
}