#include <fmt/format.h>
#include <gtest/gtest.h>
#include <algorithm>
#include <bitset>
#include <iostream>
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
    // r.reserve(N);
    r.resize(N);
    std::generate(r.begin(), r.end(), [&]() { return map[dist(gen)]; });
    return r;
}

TEST(SeqEncode, Compress)
{
    /*
     * acgt
     * 0123
     * 0123 4567 8901 2345 6
     * acgt catg ttaa ggtt c
     * 0123 1032 3300 2233 1
     * 3322 0033 2301 3210 1
     * [01] [11 11 10 10 00 00 11 11 10 11 00 01 11 10 01 00]
     */
    const vector<uint32_t> ans = {0b11111010000011111011000111100100, 0b01};
    const string seq = "acgtcatgttaaggttc";
    const auto enc = compress(seq);
    ASSERT_EQ(enc, ans);
}

TEST(SeqEncode, Decompress)
{
    for (int N = 2; N < 31; N += 2) {
        const string seq = generate_genome_seq(N);
        ASSERT_EQ(seq, decompress(compress(seq), seq.size()));
    }
    for (int N = 1; N < 1 << 24; N <<= 2) {
        const string seq = generate_genome_seq(N);
        ASSERT_EQ(seq, decompress(compress(seq), seq.size()));
    }
}

TEST(SeqEncode, get_seq)
{
    const auto map = "acgt";
    for (int N = 1; N < 1 << 16; N <<= 2) {
        const string seq = generate_genome_seq(N);
        const auto enc = compress(seq);
        for (int i = 0; i < N; ++i) {
            auto c = get_seq(enc, i);
            ASSERT_EQ(seq[i], map[c]);
        }
    }
}