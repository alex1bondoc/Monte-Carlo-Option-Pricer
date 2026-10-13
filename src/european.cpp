#include "pricing.hpp"
#include "rng.hpp"
#include "params.hpp"
#include <algorithm>
#include <cmath>
#include <thread>
#include <vector>

namespace {
    class Partial {
    public:
        double s{};
        double sq{};
    };
}

Result european(const Params& p) {
    auto drift = (p.r - p.sigma * p.sigma / 2) * p.T;
    auto vol = p.sigma * std::sqrt(p.T);
    auto sum = 0.0;
    auto sum_sq = 0.0;
    for (int i{}; i < p.paths; ++i) {
        auto Z = normal(p.seed, i, 0);
        auto SI = p.S0 * std::exp(drift + vol * Z);
        auto payoff = std::max(p.K - SI, 0.0);
        sum += payoff;
        sum_sq += payoff * payoff;
    }

    auto mean = sum / p.paths;
    auto price = std::exp(-p.r * p.T) * mean;
    auto error = std::exp(-p.r * p.T) * std::sqrt((sum_sq / p.paths - mean * mean) / p.paths);
    return {price, error};
}

Result european_parallel(const Params& p, const int N) {
    auto drift = (p.r - p.sigma * p.sigma / 2) * p.T;
    auto vol = p.sigma * std::sqrt(p.T);
    std::vector<Partial> partials(N);
    std::vector<std::thread> threads;

    for (int t{}; t < N; ++t) { 
        threads.emplace_back([&, t, N] {
            const long begin = t * p.paths / N; 
            const long end = std::min(p.paths, (t + 1) * p.paths / N);

            double sum = 0.0;
            double sum_sq = 0.0;
            for (long i{begin}; i < end; ++i) {
                const double ST = p.S0 * std::exp(drift + vol * normal(p.seed, i, 0));
                const double payoff = std::max(p.K - ST, 0.0);
                sum += payoff;
                sum_sq += payoff * payoff;
            }
            partials[t] = {sum, sum_sq};
        });
    }
    for (auto& t : threads) t.join();
    double sum = 0.0;
    double sum_sq = 0.0;
    for (const auto& p : partials) { 
        sum += p.s;
        sum_sq += p.sq;
    }

    auto mean = sum / p.paths;
    auto price = std::exp(-p.r * p.T) * mean;
    auto error = std::exp(-p.r * p.T) * std::sqrt((sum_sq / p.paths - mean * mean) / p.paths);
    return {price, error};
}
