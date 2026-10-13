#include "params.hpp"

class Result {
public:
    double price;
    double std_err;
};

double black_scholes(const Params& p);
Result european(const Params& p);
Result european_parallel(const Params& p, const int N = 8);

Result american(const Params& p);
Result american_parallel(const Params& p, const int N = 8);

