#include "params.hpp"

class Result {
public:
    double price;
    double std_err;
};

double black_scholes(const Params& p);
Result european(const Params& p);
