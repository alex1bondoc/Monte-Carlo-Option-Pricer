#include <iostream>
#include <chrono>
#include "functions.hpp"

int main() {
    Params params{
        36.0, // S0
        40.0, // K
        0.06,  // r
        0.2,   // sigma
        1.0,   // T
        1'000'000, // paths
        365,    // steps
        42      // seed
    };

    auto start = std::chrono::high_resolution_clock::now();
    Result result = european(params);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << result.price << std::endl;
    std::cout << result.error << std::endl;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
    return 0;
}