#pragma once
#include "params.hpp"
#include "result.hpp"
#include "normal.hpp"


Result european(const Params& params);
Result european_threads(const Params& params, int num_threads = 8);
Result american(const Params& params);
Result american_threads(const Params& params, int num_threads = 8);

inline double payoff(const Params& params, double S) {
    if (params.option_type == OptionType::Call) {
        return std::max(S - params.K, 0.0);
    } else {
        return std::max(params.K - S, 0.0);
    }
}