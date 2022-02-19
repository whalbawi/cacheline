cmake_minimum_required(VERSION 3.20)
project(cacheline CXX)

set(CMAKE_CXX_STANDARD 14)

enable_testing()

# Directories
set(CACHELINE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CACHELINE_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
set(CACHELINE_BENCHMARK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/benchmark)

set(CACHELINE_THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
set(CACHELINE_GOOGLETEST_DIR ${CACHELINE_THIRD_PARTY_DIR}/googletest)
set(CACHELINE_GOOGLEBENCHMARK_DIR ${CACHELINE_THIRD_PARTY_DIR}/benchmark)

# Dependencies
add_subdirectory(${CACHELINE_GOOGLETEST_DIR} EXCLUDE_FROM_ALL)
add_subdirectory(${CACHELINE_GOOGLEBENCHMARK_DIR} EXCLUDE_FROM_ALL)

# Project version and commit
file(READ VERSION CACHELINE_PROJECT_VERSION)
string(STRIP ${CACHELINE_PROJECT_VERSION} CACHELINE_PROJECT_VERSION)
execute_process(COMMAND git describe --always --dirty
        OUTPUT_VARIABLE CACHELINE_GIT_COMMIT_HASH
        ERROR_VARIABLE CACHELINE_GIT_COMMIT_ERROR
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE CACHELINE_GIT_COMMIT_RESULT
        )
if (NOT ${CACHELINE_GIT_COMMIT_RESULT} STREQUAL 0)
    message(WARNING "Could not retrieve git commit: " ${CACHELINE_GIT_COMMIT_ERROR})
    set(CACHELINE_GIT_COMMIT_HASH unknown)
endif()
message("cacheline version information: " ${CACHELINE_PROJECT_VERSION} " - " ${CACHELINE_GIT_COMMIT_HASH})
configure_file(${CACHELINE_SRC_DIR}/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/gen/version.h @ONLY)

# Source files
set(CACHELINE_SRC_LIST
    ${CACHELINE_SRC_DIR}/cacheline.cpp
)

# Test files
set(CACHELINE_TEST_LIST
    ${CACHELINE_TEST_DIR}/matrix_test.cpp
    ${CACHELINE_TEST_DIR}/cacheline_test.cpp
)

# Benchmark files
set(CACHELINE_BENCHMARK_LIST
    ${CACHELINE_BENCHMARK_DIR}/counters_bench.cpp
    ${CACHELINE_BENCHMARK_DIR}/matrix_bench.cpp
)

add_library(cacheline-lib ${CACHELINE_SRC_LIST})
target_include_directories(cacheline-lib PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/gen)
set_target_properties(cacheline-lib PROPERTIES OUTPUT_NAME cacheline)

add_executable(cacheline-tests ${CACHELINE_TEST_LIST})
target_include_directories(cacheline-tests PRIVATE ${CACHELINE_SRC_DIR})
target_link_libraries(cacheline-tests cacheline-lib gtest_main)

add_test(cacheline-tests cacheline-tests)

add_executable(cacheline-benchmarks ${CACHELINE_BENCHMARK_LIST})
target_include_directories(cacheline-benchmarks PRIVATE ${CACHELINE_SRC_DIR})
target_link_libraries(cacheline-benchmarks cacheline-lib benchmark_main)

add_test(cacheline-benchmarks cacheline-benchmarks)
