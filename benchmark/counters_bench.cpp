#include <atomic>
#include <numeric>

#include "benchmark/benchmark.h"

template <typename T> struct alignas(64) CounterAligned {
    alignas(64) T data;
};

template <typename T> struct Counter { T data; };

int num_increments = 1000000;

template <typename T> void BM_increment(benchmark::State& state) {
    static T* counters;
    if (state.thread_index() == 0) {
        counters = new T[state.threads()]{};
    }

    benchmark::DoNotOptimize(counters);
    for (auto _ : state) {
        for (int i = 0; i < num_increments; ++i) {
            counters[state.thread_index()].data++;
            benchmark::ClobberMemory();
        }
    }

    if (state.thread_index() == 0) {
        long sum = 0;
        for (int i = 0; i < state.threads(); ++i) {
            sum += counters[i].data;
        }
        delete[] counters;
        state.counters["sum"] = sum;
    }
}

#define COUNTER_BENCHMARK(x) BENCHMARK(BM_increment<x>)->DenseThreadRange(1, 16, 1);

COUNTER_BENCHMARK(Counter<int64_t>);
COUNTER_BENCHMARK(CounterAligned<int64_t>);
// COUNTER_BENCHMARK(Counter<std::atomic<long>>);
// COUNTER_BENCHMARK(CounterAligned<std::atomic<long>>);
