#include <gtest/gtest.h>
#include "functions.hpp"
#include "black_scholes.hpp"

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

// TEST(Test, EuropeanOptionThreadsTest2) {
//     Params p{
//         36.0, // S0
//         40.0, // K
//         0.06,  // r
//         0.2,   // sigma
//         1.0,   // T
//         1'000'000, // paths
//         365,    // steps
//         42
//     };   // cu valorile implicite 36 / 40 / 0.06 / 0.2 / 1
//     Result result = european_threads(p, 2);
//     EXPECT_NEAR(result.price, 3.8443, 3 * result.error);
// }

// TEST(Test, EuropeanOptionThreadsTest4) {
//     Params p{
//         36.0, // S0
//         40.0, // K
//         0.06,  // r
//         0.2,   // sigma
//         1.0,   // T
//         1'000'000, // paths
//         365,    // steps
//         42
//     };   // cu valorile implicite 36 / 40 / 0.06 / 0.2 / 1
//     Result result = european_threads(p, 4);
//     EXPECT_NEAR(result.price, 3.8443, 3 * result.error);
// }

// TEST(Test, EuropeanOptionThreadsTest8) {
//     Params p{
//         36.0, // S0
//         40.0, // K
//         0.06,  // r
//         0.2,   // sigma
//         1.0,   // T
//         1'000'000, // paths
//         365,    // steps
//         42
//     };   // cu valorile implicite 36 / 40 / 0.06 / 0.2 / 1
//     Result result = european_threads(p);
//     EXPECT_NEAR(result.price, 3.8443, 3 * result.error);
// }

// TEST(Test, EuropeanOptionThreadsTest16) {
//     Params p{
//         36.0, // S0
//         40.0, // K
//         0.06,  // r
//         0.2,   // sigma
//         1.0,   // T
//         1'000'000, // paths
//         365,    // steps
//         42
//     };   // cu valorile implicite 36 / 40 / 0.06 / 0.2 / 1
//     Result result = european_threads(p, 16);
//     EXPECT_NEAR(result.price, 3.8443, 3 * result.error);
// }

// TEST(Test, AmericanOptionThreadsTest) {
//     Params p{
//         36.0, // S0
//         40.0, // K
//         0.06,  // r
//         0.2,   // sigma
//         1.0,   // T
//         1'000'000, // paths
//         365,    // steps
//         42
//     };   // cu valorile implicite 36 / 40 / 0.06 / 0.2 / 1
//     Result result = american_threads(p);
//     EXPECT_NEAR(result.price, 4.469, 3 * result.error);
// }