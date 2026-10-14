#include "params.hpp"
#include <cmath>

double normal_cdf(double x);
double black_scholes_call(const Params& params);
double black_scholes_put(const Params& params);
double binomial_american(const Params& params, int n = 2000);