#pragma once
#include <cstdint>

enum class OptionType {
    Call,
    Put
};

class Params {
public:
    double S0{};                // stock price today
    double K{};                 // strike (fixed price)
    double r{};                 // interest
    double sigma{};             // volatility
    double T{};                 // time (years)
    OptionType option_type{};   // type of option

    long paths{};               // number of simulations
    long steps{};               // number of days 
    uint32_t seed{};            // philox seed
};