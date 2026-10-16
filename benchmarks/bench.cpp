#include <benchmark/benchmark.h>
#include "functions.hpp"

static const Option option{
    36.0,               // S0
    40.0,               // K
    1.0,                // T
    OptionType::Call    // type
};

static const MarketData market{
    0.06,   // r
    0.2     // sigma
};

static SimulationData make_simulation(long paths, long steps) {
    SimulationData s{};
    s.paths = paths;
    s.steps = steps;
    s.seed  = 42;
    return s;
}

static void benchmark_european(benchmark::State& state) {
    const SimulationData s = make_simulation(state.range(0), state.range(1));
    for (auto _ : state) {
        benchmark::DoNotOptimize(european(option, market, s));
    }
}
BENCHMARK(benchmark_european)
    ->Args({1'000'000, 365})
    ->Unit(benchmark::kMillisecond);

static void benchmark_european_threads(benchmark::State& state) {
    const SimulationData s = make_simulation(state.range(0), state.range(1));
    const int num_threads = static_cast<int>(state.range(2));
    for (auto _ : state) {
        benchmark::DoNotOptimize(european_threads(option, market, s, num_threads));
    }
}
BENCHMARK(benchmark_european_threads)
    ->ArgsProduct({{1'000'000}, {365}, {1, 2, 4, 6, 8, 12, 14, 16, 20}})
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond);

static void benchmark_american(benchmark::State& state) {
    const SimulationData s = make_simulation(state.range(0), state.range(1));
    for (auto _ : state) {
        benchmark::DoNotOptimize(american(option, market, s));
    }
}

BENCHMARK(benchmark_american)
    ->Args({1'000'000, 365})
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond);

static void benchmark_american_threads(benchmark::State& state) {
    const SimulationData s = make_simulation(state.range(0), state.range(1));
    const int num_threads = static_cast<int>(state.range(2));
    for (auto _ : state) {
        benchmark::DoNotOptimize(american_threads(option, market, s, num_threads));
    }
}

BENCHMARK(benchmark_american_threads)
    ->ArgsProduct({{1'000'000}, {365}, {1, 2, 4, 6, 8, 12, 14, 16, 20}})
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
