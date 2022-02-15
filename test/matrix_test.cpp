#include "matrix.h"

#include <thread>

#include "gtest/gtest.h"

class MatrixTest : public ::testing::Test {
  protected:
    static const int num_rows = 5;
    static const int num_cols = 5;
    cacheline::Matrix<int> matrix{num_rows, num_cols};
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

TEST_F(MatrixTest, Partition) {
    int rows_per_block = 50;
    int num_row_blocks = 2;
    int num_rows = num_row_blocks * rows_per_block;
    int cols_per_block = 50;
    int num_col_blocks = 4;
    int num_cols = num_col_blocks * cols_per_block;

    cacheline::Matrix<int> matrix{num_rows, num_cols};
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            matrix.set(i, j, 1);
        }
    }

    std::vector<cacheline::Matrix<int>::View> blocks =
        matrix.partition(num_row_blocks, num_col_blocks);
    int sum = 0;
    for (auto&& block : blocks) {
        ASSERT_EQ(block.num_rows, rows_per_block);
        ASSERT_EQ(block.num_cols, cols_per_block);
        for (int i = 0; i < block.num_rows; ++i) {
            for (int j = 0; j < block.num_cols; ++j) {
                block.visit(i, j, [&](int val) { sum += val; });
            }
        }
    }

    ASSERT_EQ(sum, num_rows * num_cols);
}

TEST_F(MatrixTest, PartitionUneven) {
    int num_row_blocks = 2;
    int num_rows = 101;
    int num_col_blocks = 4;
    int num_cols = 17;

    cacheline::Matrix<int> matrix{num_rows, num_cols};
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            matrix.set(i, j, 1);
        }
    }

    std::vector<cacheline::Matrix<int>::View> blocks =
        matrix.partition(num_row_blocks, num_col_blocks);
    int sum = 0;
    for (auto&& block : blocks) {
        ASSERT_GE(block.num_rows, num_rows / num_row_blocks);
        ASSERT_LE(block.num_rows, num_rows / num_row_blocks + 1);
        ASSERT_GE(block.num_cols, num_cols / num_col_blocks);
        ASSERT_LE(block.num_cols, num_cols / num_col_blocks + 1);
        for (int i = 0; i < block.num_rows; ++i) {
            for (int j = 0; j < block.num_cols; ++j) {
                block.visit(i, j, [&](int val) { sum += val; });
            }
        }
    }

    ASSERT_EQ(sum, num_rows * num_cols);
}
