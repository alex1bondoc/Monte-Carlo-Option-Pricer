#include <gtest/gtest.h>
#include "functions.hpp"

TEST(Test, EuropeanOptionTest) {
    Params p{
        36.0, // S0
        40.0, // K
        0.06,  // r
        0.2,   // sigma
        1.0,   // T
        1'000'000, // paths
        365,    // steps
        42
    };   // cu valorile implicite 36 / 40 / 0.06 / 0.2 / 1
    Result result = european(p);
    EXPECT_NEAR(result.price, 3.8443, 3 * result.error);
}

TEST(Test, AmericanOptionTest) {
    Params p{
        36.0, // S0
        40.0, // K
        0.06,  // r
        0.2,   // sigma
        1.0,   // T
        1'000'000, // paths
        365,    // steps
        42
    };   // cu valorile implicite 36 / 40 / 0.06 / 0.2 / 1
    Result result = american(p);
    EXPECT_NEAR(result.price, 4.469, 3 * result.error);
}

TEST(Test, EuropeanOptionThreadsTest2) {
    Params p{
        36.0, // S0
        40.0, // K
        0.06,  // r
        0.2,   // sigma
        1.0,   // T
        1'000'000, // paths
        365,    // steps
        42
    };   // cu valorile implicite 36 / 40 / 0.06 / 0.2 / 1
    Result result = european_threads(p, 2);
    EXPECT_NEAR(result.price, 3.8443, 3 * result.error);
}

TEST(Test, EuropeanOptionThreadsTest4) {
    Params p{
        36.0, // S0
        40.0, // K
        0.06,  // r
        0.2,   // sigma
        1.0,   // T
        1'000'000, // paths
        365,    // steps
        42
    };   // cu valorile implicite 36 / 40 / 0.06 / 0.2 / 1
    Result result = european_threads(p, 4);
    EXPECT_NEAR(result.price, 3.8443, 3 * result.error);
}

TEST(Test, EuropeanOptionThreadsTest8) {
    Params p{
        36.0, // S0
        40.0, // K
        0.06,  // r
        0.2,   // sigma
        1.0,   // T
        1'000'000, // paths
        365,    // steps
        42
    };   // cu valorile implicite 36 / 40 / 0.06 / 0.2 / 1
    Result result = european_threads(p);
    EXPECT_NEAR(result.price, 3.8443, 3 * result.error);
}

TEST(Test, EuropeanOptionThreadsTest16) {
    Params p{
        36.0, // S0
        40.0, // K
        0.06,  // r
        0.2,   // sigma
        1.0,   // T
        1'000'000, // paths
        365,    // steps
        42
    };   // cu valorile implicite 36 / 40 / 0.06 / 0.2 / 1
    Result result = european_threads(p, 16);
    EXPECT_NEAR(result.price, 3.8443, 3 * result.error);
}

TEST(Test, AmericanOptionThreadsTest) {
    Params p{
        36.0, // S0
        40.0, // K
        0.06,  // r
        0.2,   // sigma
        1.0,   // T
        1'000'000, // paths
        365,    // steps
        42
    };   // cu valorile implicite 36 / 40 / 0.06 / 0.2 / 1
    Result result = american_threads(p);
    EXPECT_NEAR(result.price, 4.469, 3 * result.error);
}