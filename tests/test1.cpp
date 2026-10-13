#include <gtest/gtest.h>
#include "functions.hpp"

TEST(BlackScholes, ValoareCunoscuta) {
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