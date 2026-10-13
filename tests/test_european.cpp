#include "pricing.hpp"
#include "params.hpp"
#include "rng.hpp"
#include "black_scholes.hpp"
#include <gtest/gtest.h>
#include <cmath>
#include <cstdio>

static Params base() {
    Params p{36, 40, 0.06, 0.2, 1.0, 252, 1'000'000, 42};
    return p;
}

TEST(Generator, MeanZero) {
    const long N = 1'000'000;
    double s{}, s2{};
    for (long i{}; i < N; ++i) {
        double z = normal(42, i, 0);
        s += z;
        s2 += z * z;
    }
    double mean = s / N;
    double sd = std::sqrt(s2 / N - mean * mean);
    EXPECT_NEAR(mean, 0.0, 0.005);
    EXPECT_NEAR(sd, 1.0, 0.005);
}

TEST(Generator, Persistent) {
    EXPECT_EQ(normal(42, 123, 7), normal(42, 123, 7));
    EXPECT_NE(normal(42, 123, 7), normal(43, 123, 7));
    EXPECT_NE(normal(42, 123, 7), normal(42, 123, 8));
}

TEST(European, BlackScholes) {
    EXPECT_NEAR(black_scholes(base()), 3.8443, 1e-3);
}

TEST(European, MonteCarlo1) {
    auto p = base();
    auto r = european_parallel(p);
    EXPECT_NEAR(r.price, black_scholes(p), 3 * r.std_err);
}

