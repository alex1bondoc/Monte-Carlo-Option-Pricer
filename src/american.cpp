#include "pricing.hpp"
#include "rng.hpp"
#include "params.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <thread>
#include <vector>
namespace {
    class Partial {
    public:
        double s{};
        double sq{};
    };
    
    struct Accum {
        double XtX[9] = {};   // matrice 3x3, pe rânduri
        double Xty[3] = {};
        long count = 0;
    
        void add(double x, double y) {
            const double v[3] = {1.0, x, x * x};
            for (int a = 0; a < 3; ++a) {
                Xty[a] += v[a] * y;
                for (int b = 0; b < 3; ++b) XtX[a * 3 + b] += v[a] * v[b];
            }
            ++count;
        }
    };
    
    // rezolvă XtX * beta = Xty (eliminare Gauss cu pivotare)
    std::array<double, 3> solve3(const Accum& acc) {
        double A[9], b[3];
        std::copy(acc.XtX, acc.XtX + 9, A);
        std::copy(acc.Xty, acc.Xty + 3, b);
        for (int i = 0; i < 3; ++i) {
            int piv = i;
            for (int j = i + 1; j < 3; ++j)
                if (std::abs(A[j * 3 + i]) > std::abs(A[piv * 3 + i])) piv = j;
            for (int k = 0; k < 3; ++k) std::swap(A[i * 3 + k], A[piv * 3 + k]);
            std::swap(b[i], b[piv]);
            for (int j = i + 1; j < 3; ++j) {
                const double f = A[j * 3 + i] / A[i * 3 + i];
                for (int k = i; k < 3; ++k) A[j * 3 + k] -= f * A[i * 3 + k];
                b[j] -= f * b[i];
            }
        }
        std::array<double, 3> x{};
        for (int i = 2; i >= 0; --i) {
            double s = b[i];
            for (int k = i + 1; k < 3; ++k) s -= A[i * 3 + k] * x[k];
            x[i] = s / A[i * 3 + i];
        }
        return x;
    }

}

Result american(const Params& p) {
    const double dt = p.T / p.steps;
    const double drift = (p.r - 0.5 * p.sigma * p.sigma) * dt;
    const double vol = p.sigma * std::sqrt(dt);
    const double disc = std::exp(-p.r * dt);

    // S[step * paths + path] for a specific path and steps
    std::vector<double> S(static_cast<size_t>(p.paths) * p.steps);
    for (long path{}; path < p.paths; ++path) {
        double s = p.S0;
        for (int step{}; step < p.steps; ++step) {
            s *= std::exp(drift + vol * normal(p.seed, path, step));
            S[static_cast<size_t>(step) * p.paths + path] = s;
        }
    }
    std::vector<double> cash(p.paths);
    for (long path{}; path < p.paths; ++path) {
        cash[path] = std::max(p.K - S[static_cast<size_t>(p.paths) * (p.steps - 1) + path], 0.0);
    }

    for (int step{p.steps - 2}; step >= 0; --step) {
        for (long path{}; path < p.paths; ++path) {
            cash[path] *= disc;
        }
        Accum acc{};
        for (long path{}; path < p.paths; ++path) {
            if (S[static_cast<size_t>(step) * p.paths + path] < p.K) {
                acc.add(S[static_cast<size_t>(step) * p.paths + path] / p.K, cash[path]);
            }
        }
        if (acc.count < 3) {
            continue;
        }
        auto beta = solve3(acc);
        for (long path{}; path < p.paths; ++path) {
            if (S[static_cast<size_t>(step) * p.paths + path] < p.K) {
                auto x = S[static_cast<size_t>(step) * p.paths + path] / p.K;
                auto astept = beta[0] + beta[1] * x + beta[2] * x * x;
                if (p.K - S[static_cast<size_t>(step) * p.paths + path] > astept) { 
                    cash[path] = p.K - S[static_cast<size_t>(step) * p.paths + path];
                }
            }
        }
    }

    double sum = 0.0, sum_sq = 0.0;
    for (long path = 0; path < p.paths; ++path) {
        const double v = cash[path] * disc;   // din ziua 0 până azi
        sum    += v;
        sum_sq += v * v;
    }
    const double n     = static_cast<double>(p.paths);
    const double mean  = sum / n;
    const double error = std::sqrt((sum_sq / n - mean * mean) / n);

    return {std::max(mean, p.K - p.S0), error};
}

Result american_parallel(const Params& p, const int N) {
    return Result{0.0, 0.0};
}

