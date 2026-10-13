#include <iostream>
#include <Random123/philox.h>
#include <Random123/uniform.hpp>

using RNG = r123::Philox2x32;
constexpr auto& uniform = r123::u01<double, uint32_t>;

int main() {
    RNG rng{};
    RNG::ctr_type ctr{};
    RNG::key_type key{};

    std::cout << "Random numbers generated using Philox2x32:\n";
    for (int i = 0; i < 10; ++i) {
        ctr = {static_cast<uint32_t>(i), 0};
        RNG::ctr_type random_numbers = rng(ctr, key);
        std::cout << "Random number " << i + 1 << ": ";
        std::cout << uniform(random_numbers[0]) << ", " << uniform(random_numbers[2]) << "\n";
    }
    
    return 0;
}