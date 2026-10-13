#include <iostream>
#include <Random123/philox.h>
#include <Random123/uniform.hpp>

int main() {
    r123::Philox4x32 gen;
    r123::Philox4x32::ctr_type ctr{{0, 0, 0, 0}};
    r123::Philox4x32::key_type key{{42, 0}};
    auto r = gen(ctr, key);
    std::cout << r123::u01<double>(r[0]) << "\n";
}
