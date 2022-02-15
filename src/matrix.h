#ifndef CACHELINE_MATRIX_H_
#define CACHELINE_MATRIX_H_

#include <functional>

namespace cacheline {

template <typename T, int num_rows, int num_cols> class Matrix {
  public:
    Matrix() {
        for (int i = 0; i < num_rows; ++i) {
            for (int j = 0; j < num_cols; ++j) {
                arr[i][j] = 0;
            }
        }
    }

    void set(int i, int j, T val) {
        arr[i][j] = val;
    }
    void visit(int i, int j, std::function <void(T t)> fn) const {
        fn(arr[i][j]);
    };

  private:
    T arr[num_rows][num_cols];
};

} // namespace cacheline

#endif // CACHELINE_MATRIX_H_
