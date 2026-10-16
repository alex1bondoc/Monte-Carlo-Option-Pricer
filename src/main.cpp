#include <iostream>
#include <chrono>
#include "functions.hpp"
#include <CLI/CLI.hpp>
#include <map>
#include <string>

int main(int argc, char** argv) {
    CLI::App app{"Monte Carlo Option Pricer"};

    Option option{40.0, 1.0, OptionType::Call};
    MarketData market{36.0, 0.06, 0.2};
    SimulationData simulation{1'000'000, 365, 42};
    
    bool american_option{};
    int num_threads{1};

    std::map<std::string, OptionType> types{{"call", OptionType::Call}, {"put", OptionType::Put}};
    app.add_option("--type", option.option_type, "call or put")
        ->transform(CLI::CheckedTransformer(types, CLI::ignore_case));
    app.add_option("-K,--strike", option.K, "strike price");
    app.add_option("-T,--time", option.T, "time till strike(years)");
    app.add_option("-S,--price", market.S0, "price of stock");
    app.add_option("-r,--interest", market.r, "interest rate");
    app.add_option("-s,--volatility", market.sigma, "volatility");
    app.add_option("-p,--paths", simulation.paths, "paths");
    app.add_option("-d,--days", simulation.steps, "days");
    app.add_option("--seed", simulation.seed, "seed");
    app.add_option("-t,--threads", num_threads, "threads");
    app.add_option("--american", american_option, "american option");

    CLI11_PARSE(app, argc, argv);

    Result res{};
    if (american_option) {
        if (num_threads == 1) {
            res = american(option, market, simulation);
        }
        else {
            res = american_threads(option, market, simulation, num_threads);
        }
    } else {
        if (num_threads == 1) {
            res = european(option, market, simulation);
        }
        else {
            res = european_threads(option, market, simulation, num_threads);
        }
    }
    std::cout << res.price << "\n";
    
    return 0;
}
