#pragma once
#include <algorithm>
#include <cmath>
#include <vector>
#include "genomeseq.h"
#include "simple_matrix.h"

template <typename T>
struct hmm {
    hmm(int num_states, int num_hidden_states)
        : A(num_hidden_states, num_hidden_states),
          B(num_states, num_hidden_states),
          Pi(1, num_hidden_states)
    {
    }

    template <typename Seq, typename Getter>
    mat2d<T> forward(const Seq seq, size_t N, Getter geti)
    {
        auto a_t = Pi.dot(B.get_row(geti(seq, 0)));
        for (int i = 1; i < N; ++i)
            a_t = (a_t * A).dot(B.get_row(geti(seq, i)).T());
        return a_t;
    }

    // backward not implemented

    template <typename Seq, typename Getter>
    mat2d<T> viterbi(const Seq seq, size_t N, Getter geti)
    {
        auto a_t = Pi.dot(B.get_row(geti(seq, 0)));
        for (int i = 1; i < N; ++i) {
            auto s = std::distance(
                a_t.data.begin(),
                std::max_element(a_t.data.begin(), a_t.data.end()));
            a_t = A.get_row(s).dot(B.get_row(geti(seq, i))) * a_t(0, s);
        }
        return a_t;
    }

    template <typename Seq, typename Getter>
    mat2d<T> viterbi_log2(const Seq seq, size_t N, Getter geti)
    {
        auto a_t = Pi.dot(B.get_row(geti(seq, 0)));  // 1 x H
        for (int j = 0; j < a_t.cols; ++j)
            a_t(0, j) = std::log2(a_t(0, j));
        for (int i = 1; i < N; ++i) {
            auto s = std::distance(
                a_t.data.begin(),
                std::max_element(a_t.data.begin(), a_t.data.end()));
            auto p = A.get_row(s).dot(B.get_row(geti(seq, i)));
            for (int j = 0; j < p.cols; ++j) {
                p(0, j) = std::log2(p(0, j));
            }
            a_t = p + a_t(0, s);
        }
        return a_t;
    }

    mat2d<T> A;   // transition matrix
    mat2d<T> B;   // emission matrix
    mat2d<T> Pi;  // initial state
};