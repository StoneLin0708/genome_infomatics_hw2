#include <benchmark/benchmark.h>
#include <fmt/format.h>
#include <algorithm>
#include <bitset>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include "genomeseq.h"

using fmt::print;
using std::string;
using std::vector;

string generate_genome_seq(size_t N)
{
    static auto gen = std::mt19937(87);
    static auto dist = std::uniform_int_distribution<char>(0, 3);
    static auto map = "acgt";
    string r;
    r.resize(N);
    std::generate(r.begin(), r.end(), [&]() { return map[dist(gen)]; });
    return r;
}

vector<uint32_t> generate_encoding(size_t N)
{
    static auto gen = std::mt19937(87);
    static auto dist = std::uniform_int_distribution<uint32_t>(
        0, std::numeric_limits<uint32_t>::max());
    vector<uint32_t> r(N / 16 + 1);
    std::generate(r.begin(), r.end(), [&]() { return dist(gen); });
    return r;
}

constexpr int repeat = 128;

static void BM_Compress(benchmark::State &state)
{
    for (auto _ : state) {
        state.PauseTiming();
        const string seq = generate_genome_seq(state.range(0));
        state.ResumeTiming();
        for (int i = 0; i < repeat; ++i)
            benchmark::DoNotOptimize(compress(seq));
    }
    state.SetComplexityN(state.range(0));
}

static void BM_Decompress(benchmark::State &state)
{
    for (auto _ : state) {
        state.PauseTiming();
        const auto enc = generate_encoding(state.range(0));
        state.ResumeTiming();
        for (int i = 0; i < repeat; ++i)
            benchmark::DoNotOptimize(decompress(enc, state.range(0)));
    }
    state.SetComplexityN(state.range(0));
}

static void BM_CompressNew(benchmark::State &state)
{
    for (auto _ : state) {
        state.PauseTiming();
        const string seq = generate_genome_seq(state.range(0));
        state.ResumeTiming();
        for (int i = 0; i < repeat; ++i)
            benchmark::DoNotOptimize(compress_new(seq));
    }
    state.SetComplexityN(state.range(0));
}

static void BM_DecompressNew(benchmark::State &state)
{
    for (auto _ : state) {
        state.PauseTiming();
        const auto enc = generate_encoding(state.range(0));
        state.ResumeTiming();
        for (int i = 0; i < repeat; ++i)
            benchmark::DoNotOptimize(decompress_new(enc, state.range(0)));
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_Compress)
    ->RangeMultiplier(4)
    ->Range(64, 1 << 24)
    ->Complexity(benchmark::oN);
BENCHMARK(BM_Decompress)
    ->RangeMultiplier(4)
    ->Range(64, 1 << 24)
    ->Complexity(benchmark::oN);
BENCHMARK(BM_CompressNew)
    ->RangeMultiplier(4)
    ->Range(64, 1 << 24)
    ->Complexity(benchmark::oN);
BENCHMARK(BM_DecompressNew)
    ->RangeMultiplier(4)
    ->Range(64, 1 << 24)
    ->Complexity(benchmark::oN);
BENCHMARK_MAIN();