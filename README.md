# mpp (matrix++)
A C++20 and later matrix library

![Build Status](https://img.shields.io/azure-devops/build/samestimable2016/mpp/3/main?label=%F0%9F%94%A8%20Build%20Status) ![Test Results](https://img.shields.io/azure-devops/tests/samestimable2016/mpp/3/main?label=%F0%9F%A7%AA%20Test%20Results) ![Code Coverage](https://img.shields.io/azure-devops/coverage/samestimable2016/mpp/3/main?label=%F0%9F%93%B6%20Code%20Coverage) ![GitHub Pages](https://img.shields.io/github/deployments/sam20908/mpp/github-pages?label=%F0%9F%9A%80%20GitHub%20Pages)

# How to use?

## Including the Project:
All you need to do is just add it as a subdirectory and link the target (see below):
```cmake
add_subdirectory("mpp")

target_link_libraries(your_target mpp::mpp)
```

# About Built-In Suites

## Using Built-In Tests:
CMake option `MPP_BUILD_TESTS` builds all tests by default. You can toggle unit tests and compile tests via their corresponding options.
- `MPP_BUILD_UNIT_TESTS` for unit tests.
- `MPP_BUILD_COMPILE_TESTS` for compile tests.

It has the following dependencies:
- `gtest` installed via `vcpkg` submodule for unit tests.
- LLVM Lit Test Runner `lit` installed and found in **PATH**. **It is only required for ___compile tests___!**

## Using Built-In Benchmarks:
CMake option `MPP_BUILD_BENCHMARKS` builds both runtime benchmarks and (upcoming) compile time benchmarks. You can toggle individual runtime benchmarks and compile time benchmarks via their corresponding options.
- `MPP_BUILD_RUNTIME_BENCHMARKS` for runtime benchmarks.
- `MPP_BUILD_COMPILE_TIME_BENCHMARKS` for compile time benchmarks.

It has the following dependencies:
- `benchmark` installed via `vcpkg` submodule for runtime benchmarks.
- (upcoming) `ruby` installed and found in **PATH** for compile benchmarks.

## Using Code Coverage:
CMake option `MPP_CODE_COVERAGE` defines the target `mpp_code_coverage`, which will output `mpp_code_coverage.xml` in the build folder. It has the following conditions to be met.
- `gcov` and `gcovr` installed and found in **PATH**.
- Requires unit tests to be enabled.

## Execute the tests

### FAQ: Why LLVM Lit?
LLVM Lit Test Infrastructure helps developing test suites and benchmarks easily. It also allows combination of tests and benchmarks to be run at the same time given its flexibility. mpp's CMake scripts are also designed to integrate tests with LLVM Lit to help debug tests.