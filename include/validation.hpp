#include "params.hpp"
#include <cmath>

double normal_cdf(double x);
double black_scholes_call(const Option& option, const MarketData& market, const SimulationData& simulation);
double black_scholes_put(const Option& option, const MarketData& market, const SimulationData& simulation);
double binomial_american(const Option& option, const MarketData& market, const SimulationData& simulation);
