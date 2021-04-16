#include "genomeseq.h"
#include <cmath>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

using std::string;
using std::vector;

string read_as_str(const string path)
{
    std::ifstream ifs(path);
    string data;

    ifs.seekg(0, std::ios::end);
    data.reserve(ifs.tellg());
    ifs.seekg(0, std::ios::beg);

    data.assign((std::istreambuf_iterator<char>(ifs)),
                std::istreambuf_iterator<char>());
    return data;
}


/*
 * a 110 0001 00
 * c 110 0011 01
 * g 110 0111 10
 * t 111 0100 11
 */

vector<uint32_t> compress(const string &data)
{
    // it could be done much faster with bitmask, but ...
    const auto N = data.size();
    vector<uint32_t> res(N / 16 + 1);
    for (int i = 0; i < N / 16; ++i) {
        auto &r = res[i];
        for (int j = 0; j < 16; ++j) {
            const char &c = data[i * 16 + j];
            if (c == 'a') {
            } else if (c == 'c') {
                r |= (uint32_t) 1 << (j * 2);
            } else if (c == 'g') {
                r |= (uint32_t) 2 << (j * 2);
            } else {  // (c=='t')
                r |= (uint32_t) 3 << (j * 2);
            }
        }
    }
    if (N & 15) {
        const int i = N / 16;
        auto &r = res[i];
        for (int j = 0; j < N % 16; ++j) {
            const char &c = data[i * 16 + j];
            if (c == 'a') {
            } else if (c == 'c') {
                r |= (uint32_t) 1 << (j * 2);
            } else if (c == 'g') {
                r |= (uint32_t) 2 << (j * 2);
            } else {  // (c=='t')
                r |= (uint32_t) 3 << (j * 2);
            }
        }
    }
    return res;
}

string decompress(const vector<uint32_t> &data, size_t N)
{
    const char map[] = {'a', 'c', 'g', 't'};
    string res;
    res.resize(N);
    for (int i = 0; i < N; ++i)
        res[i] = map[get_seq(data, i)];
    return res;
}

/*
 * a 110 0001 00
 * c 110 0011 01
 * t 111 0100 10
 * g 110 0111 11
 */

vector<uint32_t> compress_new(const string &data)
{
    // it could be done much faster with bitmask, but ...
    const auto N = data.size();
    vector<uint32_t> res(N / 16 + 1, 0);
    for (int i = 0; i < N / 16; ++i) {
        auto &r = res[i];
        auto v0 = *(uint64_t *) &data[i * 16] >> 1;  // 8 * 8
        for (int k = 0; k < 8; ++k, v0 >>= 8)
            r |= (v0 & 0x3) << 2 * k;
        auto v1 = *(uint64_t *) &data[i * 16 + 8] >> 1;  // 8 * 8
        for (int k = 8; k < 16; ++k, v1 >>= 8)
            r |= (v1 & 0x3) << 2 * k;
    }
    if (N & 15) {
        const int i = N / 16;
        auto &r = res[i];
        for (int j = 0; j < N % 16; ++j)
            r |= (3 & (data[i * 16 + j] >> 1)) << (j * 2);
    }
    return res;
}

string decompress_new(const vector<uint32_t> &data, size_t N)
{
    const char map[] = {'a', 'c', 't', 'g'};
    string res;
    res.resize(N);
    for (int i = 0; i < N; ++i)
        res[i] = map[get_seq(data, i)];
    return res;
}