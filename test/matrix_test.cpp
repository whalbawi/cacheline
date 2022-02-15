#include "matrix.h"

#include "gtest/gtest.h"

class MatrixTest : public ::testing::Test {
  protected:
    static const int num_rows = 5;
    static const int num_cols = 5;
    cacheline::Matrix<int, num_rows, num_cols> matrix;
};

TEST_F(MatrixTest, Initialization) {
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            matrix.set(i, j, i + j);
        }
    }

    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            matrix.visit(i, j, [i, j](int v) { ASSERT_EQ(v, i + j); });
        }
    }
}
