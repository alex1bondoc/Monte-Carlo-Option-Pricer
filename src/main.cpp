#include <iostream>
#include "pricing.hpp"
#include "params.hpp"
#include "black_scholes.hpp"

int main() {
    Params p{};
    Result r = american(p);

    std::cout << "MC: " << r.price << " +/- " << r.std_err << std::endl;
    std::cout << "Black-Scholes: " << black_scholes(p) << std::endl;
}
