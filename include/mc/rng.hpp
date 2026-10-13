#pragma once
#include <Random123/philox.h>
#include <Random123/uniform.hpp>
#include <cstdint>
#include <cmath>
#include <numbers>

inline double normal(uint32_t seed, long path, int step) {
    r123::Philox4x32 gen; 
    r123::Philox4x32::ctr_type ctr{{path, step, 0, 0}};
    r123::Philox4x32::key_type key{{seed, 0}};

    r123::Philox4x32::ctr_type r = gen(ctr, key);

    double u1 = r123::u01<double>(r[0]);
    double u2 = r123::u01<double>(r[1]);

    return std::sqrt(-2.0 * std::log(u1)) * std::cos(2.0 * std::numbers::pi * u2);
}
