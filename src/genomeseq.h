#pragma once
#include <cstdint>
#include <string>
#include <vector>

/*
 * order 0 from 0 ... 1
 * 00
 * 01
 * 10
 * 11
 *
 * order 1 from 0 ... 4
 *       00   01   10   11
 * 00  0000 0100
 * 01  0001
 * 10  0010
 * 11  0011
 *
 * order 2 from 0 ... 15
 *           00 01 10 11
 * 0000  000000
 * 0001  000001
 * 0010  000010
 * 0011  000011
 *
 */

template <typename Transform>
std::vector<double> to_prob(const std::vector<int> &x, int order, Transform F)
{
    const int k = 1 << (2 * order);
    std::vector<double> probs(4 << (2 * order));
    for (int from = 0; from < k; ++from) {
        double sum = 0;
        for (int to = 0; to < 4; ++to)
            sum += x[to * k + from];
        for (int to = 0; to < 4; ++to)
            probs[to * k + from] = F(x[to * k + from] / sum);
    };
    return probs;
}

std::vector<uint32_t> compress(const std::string &data);

std::string decompress(const std::vector<uint32_t> &data, size_t N);

std::string read_as_str(const std::string path);

inline uint8_t get_seq(const std::vector<uint32_t> &seq, size_t i)
{
    return 3 & (seq[i / 16] >> ((i % 16) * 2));
}
