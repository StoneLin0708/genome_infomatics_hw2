#include <fmt/format.h>
#include <fmt/ranges.h>
#include <cstdint>
#include <string>
#include <vector>
#include "genomeseq.h"

using fmt::print;
using std::array;
using std::string;
using std::vector;

vector<int> markov_model(const vector<uint32_t> &seq,
                         const size_t N,
                         const int order)
{
    const int k = 4 << (2 * order);
    const uint16_t mask = k - 1;
    vector<int> model(k);
    std::fill(model.begin(), model.end(), 0);
    for (int i = 0; i < N - order; ++i) {
        const int64_t v = *(int64_t *) &seq[i / 16];
        model[mask & (v >> ((i % 16) * 2))] += 1;
    }
    return model;
}

double markov_model_pred(const vector<double> &model,
                         const vector<uint32_t> &seq,
                         const size_t N,
                         const int order)
{
    const int k = 4 << (2 * order);
    const uint16_t mask = k - 1;
    double prob = 0;
    for (int i = 0; i < N - order; ++i) {
        const int64_t v = *(int64_t *) &seq[i / 16];
        prob += model[mask & (v >> ((i % 16) * 2))];
    }
    return prob;
}


int main(int argc, char *argv[])
{
    if (argc != 3) {
        print("./mm TRAIN TEST\n");
    }
    const string train = read_as_str(argv[1]);
    const string test = read_as_str(argv[2]);
    // encode a c g t to 0, 1, 2, 3 and store as a bit array
    const auto train_c = compress_new(train);
    const auto test_c = compress_new(test);
    // print("{}->{}\n", train.size(), train_c.size());

    for (int order = 0; order <= 2; ++order) {
        print("order {}:\n", order);
        const auto md = markov_model(train_c, train.size(), order);
        // print("{}\n", md);
        const auto md_log =
            to_prob(md, order, [](double x) { return std::log2(x); });
        // print("{}\n", md_log);
        auto ptrain = markov_model_pred(md_log, train_c, train.size(), order);
        print("train log probability : {:.2f}\n", ptrain);
        auto ptest = markov_model_pred(md_log, test_c, test.size(), order);
        print("test  log probability : {:.2f}\n", ptest);
    }
}