#pragma once
#include <algorithm>
#include <cstdint>

enum class OptionType {
    Call,
    Put
};

class Option {
public:
    double S0{};                // stock price today
    double K{};                 // strike (fixed price)
    double T{};
    OptionType option_type{};

    double payoff(const double S) const {
        return (option_type == OptionType::Call) ? std::max(S - K, 0.0)
                                                : std::max(K - S, 0.0);
    }
};

class MarketData {
public:
    double r{};
    double sigma{};
};

class SimulationData {
public:
    long paths{};
    long steps{};
    uint32_t seed{};
};

class Result {
public:
    double price{};
    double error{};
};
