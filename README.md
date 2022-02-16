# Cacheline 
Cache line contention experiments.

## Prerequisites
The project is built using [CMake](https://cmake.org/) and requires version 3.20 or newer to work correctly.

Cacheline takes a dependency on:
* [googletest](https://github.com/google/googletest) for unit tests
* [benchmark](https://github.com/google/benchmark) for benchmarks

Both dependencies are included in the source tree as a `git` submodule. Run the following to check out the module and
make it available to the build system:
```bash
$ cd <path-to-cacheline>
$ git submodule update --init
```

## Building
Cacheline is tested on Linux and macOS.

The following will generate the build system and then build all targets:
```bash
$ cd <path-to-cacheline>
$ cmake -B build
$ cmake --build build
```

## Benchmarks
Start by building the benchmarks executable:
```bash
$ cmake --build build --target cacheline-benchmarks
```

Now run the benchmarks using CMake's `ctest` utility:
```bash
$ ctest --test-dir build -R cacheline-benchmarks -V
```

Alternatively, run the benchmarks executable directly:
```bash
$ ./build/cacheline-benchmarks
```
## Tests
Start by building the unit tests executable:
```bash
$ cmake --build build --target cacheline-tests
```

Now run the unit tests using CMake's `ctest` utility:
```bash
$ ctest --test-dir build -R cacheline-tests -V
```

Alternatively, run the tests executable directly:
```bash
$ ./build/cacheline-tests
```
