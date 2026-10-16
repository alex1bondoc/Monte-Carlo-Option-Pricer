#pragma once
#include <algorithm>
#include <cstdint>

enum class OptionType {
    Call,
    Put
};

class Option {
public:
    double K{};                 // strike (fixed price)
    double T{};                 // time till strike in years
    OptionType option_type{};

    double payoff(const double S) const {
        return (option_type == OptionType::Call) ? std::max(S - K, 0.0)
                                                : std::max(K - S, 0.0);
    }
};

class MarketData {
public:
    double S0{};                // stock price today
    double r{};                 // interest 
    double sigma{};             // volatility
};

class SimulationData {
public:
    long paths{};               // number of simultaion
    long steps{};               // days of simulation
    uint32_t seed{};            // seed for random number
};

class Result {
public:
    double price{};
    double error{};
};
