#include <gtest/gtest.h>
#include <vector>
#include "hidden_markov_model.h"

using std::vector;

TEST(hmm, forward)
{
    hmm<float> m(3, 2);
    // clang-format off
    m.A.data = 
       {.7,.3,
        .4,.6};
    m.B.data = 
        {.5,.1,
        .4,.3,
        .1,.6};
    m.Pi.data = {.6, .4};
    // clang-format on
    vector<int> seq = {0, 1, 2};
    auto s = m.forward(seq, 3, [](auto s, int i) { return s[i]; });
    ASSERT_FLOAT_EQ(s.data[0], 0.0076960004);
    ASSERT_FLOAT_EQ(s.data[1], 0.028584003);
}

TEST(hmm, viterbi)
{
    hmm<float> m(3, 2);
    // clang-format off
    m.A.data = 
       {.7,.3,
        .4,.6};
    m.B.data = 
        {.5,.1,
        .4,.3,
        .1,.6};
    m.Pi.data = {.6, .4};
    // clang-format on
    vector<int> seq = {0, 1, 2};
    auto s = m.viterbi(seq, 3, [](auto s, int i) { return s[i]; });
    ASSERT_FLOAT_EQ(s.data[0], 0.0058800005);
    ASSERT_FLOAT_EQ(s.data[1], 0.0151200015);
}