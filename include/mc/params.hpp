#pragma once
#include <cstdint>
class Params {
public: 
    double S0 = 36.0;
    double K = 40.0;
    double r = 0.06;
    double sigma = 0.2;
    double T = 1.0;
    int steps = 252;
    long paths = 1'000'000;
    uint32_t seed = 100;
};
