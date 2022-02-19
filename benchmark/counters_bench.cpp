#include <atomic>

#include "benchmark/benchmark.h"

template <typename T> struct alignas(64) CounterAligned { T data; };

template <typename T> struct Counter { T data; };

int num_increments = 1 * 1024;

template <typename T> void BM_increment(benchmark::State& state) {
    static T* counters;
    if (state.thread_index() == 0) {
        counters = new T[state.threads()];
    }

    for (auto _ : state) {
        for (int i = 0; i < num_increments; ++i) {
            counters[state.thread_index()].data++;
            benchmark::ClobberMemory();
        }
    }

    if (state.thread_index() == 0) {
        delete[] counters;
    }
}

BENCHMARK(BM_increment<Counter<long>>)->DenseThreadRange(1, 8, 1);

BENCHMARK(BM_increment<CounterAligned<long>>)->DenseThreadRange(1, 8, 1);

BENCHMARK(BM_increment<Counter<std::atomic<long>>>)->DenseThreadRange(1, 8, 1);

BENCHMARK(BM_increment<CounterAligned<std::atomic<long>>>)->DenseThreadRange(1, 8, 1);
