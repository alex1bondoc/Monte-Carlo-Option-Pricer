#include "params.hpp"
#include <iostream>
#include <numbers>

double norm_cdf(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2.0));
}

double black_scholes(const Params& p) {
    double d1 = (std::log(p.S0 / p.K) + p.r + 0.5 * p.sigma * p.sigma) * p.T);
    double d2 = d1 - p.sigma * std::sqrt(p.T);
    return p.K * std::exp(-p.r * p.T) * norm_cdf(d1) - std::exp(-p.r * p.T) * norm_cdf(d2);
}
