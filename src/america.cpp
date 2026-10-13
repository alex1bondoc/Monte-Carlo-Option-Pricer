#include "functions.hpp"
#include <vector>

Result american(const Params& params) {
    const auto dt = params.T / params.steps;
    const auto drift = (params.r - 0.5 * params.sigma * params.sigma) * dt;
    const auto volatility = params.sigma * std::sqrt(dt);

    size_t N{params.paths};
    size_t M{params.steps};

    // If step * paths + path all paths at the same step on cache
    std::vector<double> S(N * M);
    for (int path = 0; path < N; ++path) {
        
    }

    return Result{0.0, 0.0};
}