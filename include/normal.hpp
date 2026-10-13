#pragma once
#include <Random123/philox.h>
#include <Random123/uniform.hpp>
#include <cmath>
#include <numbers>
#include <cstdint>

using RNG = r123::Philox4x32;
constexpr auto& uniform = r123::u01<double, uint32_t>;

inline double normal(uint32_t seed, uint32_t path, uint32_t step) {
    RNG rng{};
    RNG::ctr_type ctr{{path, step, 0, 0}};
    RNG::key_type key{{seed, 0}};
    RNG::ctr_type random_numbers = rng(ctr, key);

    auto u1 = uniform(random_numbers[0]);
    auto u2 = uniform(random_numbers[1]);

    return std::sqrt(-2.0 * std::log(u1)) * std::cos(2.0 * std::numbers::pi * u2);
}