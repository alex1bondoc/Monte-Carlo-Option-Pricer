#pragma once
#include <cstdint>

enum class OptionType {
    Call,
    Put
};

class Params {
public:
    double S0{};
    double K{};
    double r{};
    double sigma{};
    double T{};
    OptionType option_type{};

    long paths{};
    long steps{};
    uint32_t seed{};
};