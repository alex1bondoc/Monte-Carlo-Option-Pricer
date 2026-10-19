#include <gtest/gtest.h>
#include "functions.hpp"
#include "params.hpp"
#include "validation.hpp"

static Option make_option(OptionType type) {
    return Option{
        40.0,   // K
        1.0,    // T
        type    // type
    };
}

static const MarketData market{
    36.0,   // S0
    0.06,   // r
    0.2     // sigma
};

static const SimulationData european_simulation{
    1'000'000,  // paths
    365,        // steps
    42          // seed
};

static const SimulationData american_simulation{
    100'000,    // paths
    50,         // steps
    42          // seed
};

TEST(BlackScholes, Call) {
    const Option option = make_option(OptionType::Call);
    EXPECT_NEAR(black_scholes_call(option, market, european_simulation), 2.1737, 1e-4);
}

TEST(BlackScholes, Put) {
    const Option option = make_option(OptionType::Put);
    EXPECT_NEAR(black_scholes_put(option, market, european_simulation), 3.8443, 1e-4);
}

TEST(European, Call) {
    const Option option = make_option(OptionType::Call);
    Result result = european(option, market, european_simulation);
    EXPECT_NEAR(result.price, black_scholes_call(option, market, european_simulation), 3 * result.error);
}

TEST(European, Put) {
    const Option option = make_option(OptionType::Put);
    Result result = european(option, market, european_simulation);
    EXPECT_NEAR(result.price, black_scholes_put(option, market, european_simulation), 3 * result.error);
}

TEST(European_Threads, Put) {
    const Option option = make_option(OptionType::Put);
    Result result = european_threads(option, market, european_simulation);
    EXPECT_NEAR(result.price, black_scholes_put(option, market, european_simulation), 3 * result.error);
}

TEST(European_Threads, Call) {
    const Option option = make_option(OptionType::Call);
    Result result = european_threads(option, market, european_simulation);
    EXPECT_NEAR(result.price, black_scholes_call(option, market, european_simulation), 3 * result.error);
}

TEST(American, Put) {
    const Option option = make_option(OptionType::Put);
    Result result = american(option, market, american_simulation);
    EXPECT_NEAR(result.price, 4.478, 3 * result.error);
    EXPECT_GT(result.price, black_scholes_put(option, market, american_simulation));
    EXPECT_GE(result.price, option.K - market.S0);
}

TEST(American, Call) {
    const Option option = make_option(OptionType::Call);
    Result result = american(option, market, american_simulation);
    EXPECT_NEAR(result.price, 2.1708, 3 * result.error);
}

TEST(American_Threads, Call) {
    const Option option = make_option(OptionType::Call);
    Result result = american_threads(option, market, american_simulation);
    EXPECT_NEAR(result.price, 2.1708, 3 * result.error);
}

TEST(American_Threads, Put) {
    const Option option = make_option(OptionType::Put);
    Result result = american_threads(option, market, american_simulation);
    EXPECT_NEAR(result.price, 4.478, 3 * result.error);
    EXPECT_GT(result.price, black_scholes_put(option, market, american_simulation));
    EXPECT_GE(result.price, option.K - market.S0);
}

TEST(Consistency, European) {
    const Option option = make_option(OptionType::Put);
    Result result1 = european(option, market, european_simulation);
    Result resutl2 = european_threads(option, market, european_simulation);
    EXPECT_NEAR(result1.price, resutl2.price, 1e-10);
}

TEST(Consistency, American) {
    const Option option = make_option(OptionType::Put);
    Result result1 = american(option, market, european_simulation);
    Result resutl2 = american_threads(option, market, european_simulation);
    EXPECT_NEAR(result1.price, resutl2.price, 1e-10);
}

TEST(Consistency, AmericanOTM) {
    const Option option{30.0, 1.0, OptionType::Put};
    const SimulationData sim{20'000, 50, 42};
    EXPECT_NEAR(american(option, market, sim).price,
                american_threads(option, market, sim, 4).price, 1e-10);
}
