#include <benchmark/benchmark.h>
#include "functions.hpp"

static Params make_params(long paths, long steps) {
    Params p{};
    p.S0 = 36.0; p.K = 40.0; p.r = 0.06; p.sigma = 0.2; p.T = 1.0;
    p.paths = paths;
    p.steps = steps;
    p.seed  = 42;
    return p;
}

static void benchmark_european(benchmark::State& state) {
    const Params p = make_params(state.range(0), state.range(1));
    for (auto _ : state) {
        benchmark::DoNotOptimize(european(p));
    }
}
BENCHMARK(benchmark_european)
    ->Args({100'000, 252})
    ->Unit(benchmark::kMillisecond);

static void benchmark_european_threads(benchmark::State& state) {
    const Params p = make_params(state.range(0), state.range(1));
    const int num_threads = static_cast<int>(state.range(2));
    for (auto _ : state) {
        benchmark::DoNotOptimize(european_threads(p, num_threads));
    }
}
BENCHMARK(benchmark_european_threads)
    ->ArgsProduct({{100'000}, {252}, {2, 4, 6, 8, 12, 14, 16, 20}})
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();