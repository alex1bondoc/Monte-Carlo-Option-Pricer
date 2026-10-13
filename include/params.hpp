#pragma once
#include <cstdint>

class Params {
public:
    double S0{};
    double K{};
    double r{};
    double sigma{};
    double T{};

    long paths{};
    long steps{};
    uint32_t seed{};
};