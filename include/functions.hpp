#pragma once
#include "params.hpp"
#include "normal.hpp"


Result european(const Option& option, const MarketData& market, const SimulationData& simulation);
Result european_threads(const Option& option, const MarketData& market, const SimulationData& simulation, int num_threads = 8);
Result american(const Option& option, const MarketData& market, const SimulationData& simulation);
Result american_threads(const Option& option, const MarketData& market, const SimulationData& simulation, int num_threads = 8);
