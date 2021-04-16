#include "hidden_markov_model.h"
#include <fmt/format.h>
#include <algorithm>
#include <cstdint>
#include <random>
#include <string>
#include <vector>
#include "genomeseq.h"
#include "simple_matrix.h"

using fmt::print;
using std::array;
using std::string;
using std::vector;

template <typename T>
void col_as_prob(mat2d<T> &x)
{
    for (int i = 0; i < x.rows; ++i) {
        double sum = 0;
        for (int j = 0; j < x.cols; ++j)
            sum += x(i, j);
        for (int j = 0; j < x.cols; ++j)
            x(i, j) /= sum;
    };
}

template <typename T>
void row_as_prob(mat2d<T> &x)
{
    for (int j = 0; j < x.cols; ++j) {
        double sum = 0;
        for (int i = 0; i < x.rows; ++i)
            sum += x(i, j);
        for (int i = 0; i < x.rows; ++i)
            x(i, j) /= sum;
    };
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        print("./hmm TRAIN TEST\n");
    }
    const string train = read_as_str(argv[1]);
    const string test = read_as_str(argv[2]);

    hmm<float> m(4, 12);
    // encode a c g t to 0, 1, 2, 3 and store as a bit array
    const auto train_c = compress(train);
    const auto test_c = compress(test);
    // print("{}->{}\n", train.size(), train_c.size());

    std::mt19937 gen(10);
    std::uniform_real_distribution<double> dist(0, 1);

    auto &A = m.A.data;
    auto &B = m.B.data;
    auto &P = m.Pi.data;

    std::generate(A.begin(), A.end(), [&]() { return dist(gen); });
    col_as_prob(m.A);
    // print("{}\n", A);
    std::generate(B.begin(), B.end(), [&]() { return dist(gen); });
    row_as_prob(m.B);
    // print("{}\n", B);
    std::generate(P.begin(), P.end(), [&]() { return dist(gen); });
    col_as_prob(m.Pi);
    // print("{}\n", P);

    {
        auto s = m.viterbi_log2(train_c, 3, get_seq);
        print("train log probability {:.2f}\n",
              std::accumulate(s.data.begin(), s.data.end(), 0.0));
    }
    {
        auto s = m.viterbi_log2(test_c, 3, get_seq);
        print("test log probability {:.2f}\n",
              std::accumulate(s.data.begin(), s.data.end(), 0.0));
    }
}