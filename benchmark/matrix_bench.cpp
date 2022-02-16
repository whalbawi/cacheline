#include "matrix.h"

#include <vector>

#include "benchmark/benchmark.h"

template <int num_rows, int num_cols> class MatrixFixture : public benchmark::Fixture {
  public:
    void SetUp(const benchmark::State& state) override {
        if (state.thread_index() != 0) return;

        for (int i = 0; i < matrix.num_rows; ++i) {
            for (int j = 0; j < matrix.num_cols; ++j) {
                matrix.set(i, j, 1);
            }
        }

        blocks = matrix.partition(state.threads(), 1);
        partial_sums = std::vector<long>(state.threads(), 0);
    }

  protected:
    void sum_block_local_update(int idx) {
        const cacheline::Matrix<long>::View& block = blocks[idx];
        for (int i = 0; i < block.num_rows; ++i) {
            for (int j = 0; j < block.num_cols; ++j) {
                long entry = 0;
                block.visit(i, j, [&entry](long val) { entry = val; });
                partial_sums[idx] += entry;
            }
        }
    }

    void sum_block_global_update(int idx) {
        long sum = 0;
        const cacheline::Matrix<long>::View& block = blocks[idx];
        for (int i = 0; i < block.num_rows; ++i) {
            for (int j = 0; j < block.num_cols; ++j) {
                long entry = 0;
                block.visit(i, j, [&entry](long val) { entry = val; });
                sum += entry;
            }
        }
        partial_sums[idx] = sum;
    }

    cacheline::Matrix<long> matrix{num_rows, num_cols};
    std::vector<cacheline::Matrix<long>::View> blocks;
    std::vector<long> partial_sums;
};

BENCHMARK_TEMPLATE_DEFINE_F(MatrixFixture, MatrixSumEntriesLocal, 16 * 1024, 32 * 1024)
(benchmark::State& state) {
    for (auto _ : state) {
        sum_block_local_update(state.thread_index());
    }
}

BENCHMARK_REGISTER_F(MatrixFixture, MatrixSumEntriesLocal)
    ->Unit(benchmark::kMillisecond)
    ->DenseThreadRange(1, 16, 1);

BENCHMARK_TEMPLATE_DEFINE_F(MatrixFixture, MatrixSumEntriesGlobal, 16 * 1024, 32 * 1024)
(benchmark::State& state) {
    for (auto _ : state) {
        sum_block_global_update(state.thread_index());
    }
}

BENCHMARK_REGISTER_F(MatrixFixture, MatrixSumEntriesGlobal)
    ->Unit(benchmark::kMillisecond)
    ->DenseThreadRange(1, 16, 1);
