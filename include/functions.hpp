#pragma once
#include "params.hpp"
#include "result.hpp"
#include "normal.hpp"


Result european(const Params& params);
Result european_threads(const Params& params, int num_threads = 8);
Result american(const Params& params);
Result american_threads(const Params& params, int num_threads = 8);