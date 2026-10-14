#include <gtest/gtest.h>
#include "functions.hpp"
#include "validation.hpp"

TEST(BlackScholes, Call) {
    Params p{
        36.0,             // S0
        40.0,             // K
        0.06,             // r
        0.2,              // sigma
        1.0,              // T
        OptionType::Call, // type
        1'000'000,          // paths (nu contează pentru formulă)
        365,               // steps (nu contează pentru formulă)
        42                // seed  (nu contează pentru formulă)
    };
    EXPECT_NEAR(black_scholes_call(p), 2.1737, 1e-4);
}

TEST(BlackScholes, Put) {
    Params p{
        36.0,             // S0
        40.0,             // K
        0.06,             // r
        0.2,              // sigma
        1.0,              // T
        OptionType::Put,  // type
        1'000'000,          // paths
        365,               // steps
        42                // seed
    };
    EXPECT_NEAR(black_scholes_put(p), 3.8443, 1e-4);
}

TEST(European, Call) {
    Params p{
        36.0,             // S0
        40.0,             // K
        0.06,             // r
        0.2,              // sigma
        1.0,              // T
        OptionType::Call, // type
        1'000'000,          // paths
        365,               // steps
        42                // seed
    };
    Result result = european(p);
    EXPECT_NEAR(result.price, black_scholes_call(p), 3 * result.error);
}

TEST(European, Put) {
    Params p{
        36.0,             // S0
        40.0,             // K
        0.06,             // r
        0.2,              // sigma
        1.0,              // T
        OptionType::Put,  // type
        1'000'000,          // paths
        365,               // steps
        42                // seed
    };
    Result result = european(p);
    EXPECT_NEAR(result.price, black_scholes_put(p), 3 * result.error);
}

TEST(European_Threads, Put) {
    Params p{
        36.0,             // S0
        40.0,             // K
        0.06,             // r
        0.2,              // sigma
        1.0,              // T
        OptionType::Put,  // type
        1'000'000,          // paths
        365,               // steps
        42                // seed
    };
    Result result = european_threads(p);
    EXPECT_NEAR(result.price, black_scholes_put(p), 3 * result.error);
}

TEST(European_Threads, Call) {
    Params p{
        36.0,             // S0
        40.0,             // K
        0.06,             // r
        0.2,              // sigma
        1.0,              // T
        OptionType::Call,  // type
        1'000'000,          // paths
        365,               // steps
        42                // seed
    };
    Result result = european_threads(p);
    EXPECT_NEAR(result.price, black_scholes_call(p), 3 * result.error);
}

TEST(American, Put) {
    Params p{
        36.0,             // S0
        40.0,             // K
        0.06,             // r
        0.2,              // sigma
        1.0,              // T
        OptionType::Put,  // type
        100'000,          // paths
        50,               // steps
        42                // seed
    };
    Result result = american(p);
    EXPECT_NEAR(result.price, 4.478, 3 * result.error);
    EXPECT_GT(result.price, black_scholes_put(p));
    EXPECT_GE(result.price, p.K - p.S0);
}

TEST(American, Call) {
    Params p{
        36.0,             // S0
        40.0,             // K
        0.06,             // r
        0.2,              // sigma
        1.0,              // T
        OptionType::Call,  // type
        100'000,          // paths
        50,               // steps
        42                // seed
    };
    Result result = american(p);
    EXPECT_NEAR(result.price, 2.1708, 3 * result.error);
}

TEST(American_Threads, Call) {
    Params p{
        36.0,             // S0
        40.0,             // K
        0.06,             // r
        0.2,              // sigma
        1.0,              // T
        OptionType::Call,  // type
        100'000,          // paths
        50,               // steps
        42                // seed
    };
    Result result = american_threads(p);
    EXPECT_NEAR(result.price, 2.1708, 3 * result.error);
}

TEST(American_Threads, Put) {
    Params p{
        36.0,             // S0
        40.0,             // K
        0.06,             // r
        0.2,              // sigma
        1.0,              // T
        OptionType::Put,  // type
        100'000,          // paths
        50,               // steps
        42                // seed
    };
    Result result = american_threads(p);
    EXPECT_NEAR(result.price, 4.478, 3 * result.error);
    EXPECT_GT(result.price, black_scholes_put(p));
    EXPECT_GE(result.price, p.K - p.S0);
}