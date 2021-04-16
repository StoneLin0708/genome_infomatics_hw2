#include <gtest/gtest.h>
#include "simple_matrix.h"

using fmt::print;

TEST(SimpleMatrix, MatrixMul){
    // clang-format off
    mat2d<int> m0(2, 3);
    m0.data =
        {0, 1, 2,
         3, 4, 5};
    mat2d<int> m1(3, 2);
    m1.data = 
        {0, 1,
        -1, 1,
         0, 1};
    // clang-format on
    auto m = m0 * m1;
    ASSERT_EQ(m, mat2d<int>(2, 2, {-1, 3, -4, 12}));
}

TEST(SimpleMatrix, RowMul){
    // clang-format off
    mat2d<int> m0(2, 3);
    m0.data =
        {0, 1, 2,
         3, 4, 5};
    mat2d<int> m1(3, 2);
    m1.data = 
        {0, 1,
        -1, 1,
         0, 1};
    // clang-format on
    {
        auto m = m0.get_row(0) * m1;
        ASSERT_EQ(m, mat2d<int>(1, 2, {-1, 3}));
    }
    {
        auto m = m0.get_row(1) * m1;
        ASSERT_EQ(m, mat2d<int>(1, 2, {-4, 12}));
    }
}