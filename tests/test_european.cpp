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

TEST(European, MonteCarlo) {
    const int seeds{200};
    auto p{base()};
    const double bs{black_scholes(p)};
    int in1 = 0, in2 = 0, in3 = 0;
    double sum_z = 0;

    for (int s = 0; s < seeds; ++s) {
        p.seed = 1000 + s;
        auto r = european(p);
        double z = (r.price - bs) / r.std_err;   // câte erori standard e departe
        sum_z += z;
        if (std::abs(z) <= 1) ++in1;
        if (std::abs(z) <= 2) ++in2;
        if (std::abs(z) <= 3) ++in3;
    }
    double f1 = double(in1) / seeds, f2 = double(in2) / seeds, f3 = double(in3) / seeds;
    std::printf("in 1 SE: %.0f%%  in 2 SE: %.0f%%  in 3 SE: %.0f%%  z mediu: %.3f\n",
                100 * f1, 100 * f2, 100 * f3, sum_z / seeds);

    EXPECT_NEAR(f1, 0.68, 0.10);          // teoretic 68%
    EXPECT_GE(f2, 0.90);                  // teoretic 95%
    EXPECT_GE(f3, 0.98);                  // teoretic 99,7%
    EXPECT_NEAR(sum_z / seeds, 0.0, 0.25); // fără deplasare sistematică
}
