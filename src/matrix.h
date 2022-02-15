#ifndef CACHELINE_MATRIX_H_
#define CACHELINE_MATRIX_H_

#include <functional>
#include <vector>

namespace cacheline {

template <typename T> class Matrix {
  public:
    class View {
      public:
        void visit(int i, int j, std::function<void(T)> fn) {
            matrix.visit(row + i, col + j, fn);
        }

        const int num_rows;
        const int num_cols;

      private:
        friend class Matrix<T>;
        View(Matrix<T>& matrix, int row, int col, int num_rows, int num_cols)
            : matrix(matrix), row(row), col(col), num_rows(num_rows), num_cols(num_cols) {}

        const Matrix<T>& matrix;
        const int row;
        const int col;
    };

    Matrix(const int num_rows, const int num_cols) : num_rows(num_rows), num_cols(num_cols) {
        arr = new T* [num_rows] {};
        for (int i = 0; i < num_rows; ++i) {
            arr[i] = new T[num_cols]{};
        }
    }

    void set(int i, int j, T val) {
        arr[i][j] = val;
    }

    void visit(int i, int j, std::function<void(T)> fn) const {
        fn(arr[i][j]);
    };

    View view(int row, int col, int num_rows, int num_cols) {
        return View(*this, row, col, num_rows, num_cols);
    }

    std::vector<View> partition(int num_row_blocks, int num_col_blocks) {
        std::vector<View> views{};
        int rows_per_block = num_rows / num_row_blocks;
        int rows_rem = num_rows % rows_per_block;
        int cols_per_block = num_cols / num_col_blocks;
        int num_rows_processed = 0;
        for (int i = 0; i < num_row_blocks; ++i) {
            int add_row = std::min(1, rows_rem);
            rows_rem = std::max(0, rows_rem - 1);
            int cols_rem = num_cols % cols_per_block;
            int num_cols_processed = 0;
            for (int j = 0; j < num_col_blocks; ++j) {
                int add_col = std::min(1, cols_rem);
                cols_rem = std::max(0, cols_rem - 1);
                views.push_back(view(num_rows_processed,
                                     num_cols_processed,
                                     rows_per_block + add_row,
                                     cols_per_block + add_col));
            }
            num_rows_processed = rows_per_block + add_row;
        }

        return views;
    }

  private:
    T** arr;
    const int num_rows;
    const int num_cols;
};

} // namespace cacheline

#endif // CACHELINE_MATRIX_H_
