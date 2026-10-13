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
    const double dt    = p.T / p.steps;
    const double drift = (p.r - 0.5 * p.sigma * p.sigma) * dt;
    const double vol   = p.sigma * std::sqrt(dt);
    auto sum = 0.0;
    auto sum_sq = 0.0;
    for (int i{}; i < p.paths; ++i) {
        double s = p.S0;
        for (int t{}; t < p.steps; ++t) {
            s *=  std::exp(drift + vol * normal(p.seed, i, t));
        }
        auto payoff = std::max(p.K - s, 0.0);
        sum += payoff;
        sum_sq += payoff * payoff;
    }

    auto mean = sum / p.paths;
    auto price = std::exp(-p.r * p.T) * mean;
    auto error = std::exp(-p.r * p.T) * std::sqrt((sum_sq / p.paths - mean * mean) / p.paths);
    return {price, error};
}

Result european_parallel(const Params& p, const int N) {
    const double dt    = p.T / p.steps;
    const double drift = (p.r - 0.5 * p.sigma * p.sigma) * dt;
    const double vol   = p.sigma * std::sqrt(dt);
    std::vector<Partial> partials(N);
    std::vector<std::thread> threads;

    for (int t{}; t < N; ++t) { 
        threads.emplace_back([&, t, N] {
            const long begin = t * p.paths / N; 
            const long end = std::min(p.paths, (t + 1) * p.paths / N);

            double sum = 0.0;
            double sum_sq = 0.0;
            for (long i{begin}; i < end; ++i) {
                double s = p.S0;
                for (int j{}; j < p.steps; ++j) {
                    s *= std::exp(drift + vol * normal(p.seed, i, j));
                }
                auto payoff = std::max(p.K - s, 0.0);
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
