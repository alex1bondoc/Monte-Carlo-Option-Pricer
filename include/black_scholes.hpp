#include "params.hpp"
#include <cmath>

double normal_cdf(double x);
double black_scholes_call(const Params& params);
double black_scholes_put(const Params& params);