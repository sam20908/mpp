# matrixpp
A C++20 and later matrix library

[![Build Status](https://dev.azure.com/samestimable2016/matrixpp/_apis/build/status/sam20908.matrixpp?branchName=main)](https://dev.azure.com/samestimable2016/matrixpp/_build/latest?definitionId=2&branchName=main)

## Including the Project:
- A target `matrixpp` will be created (or `matrixpp::matrixpp` if you prefer this form). This means you can simply `add_subdirectory` the project and link the target.
- You can also just include the header into your project manually if that's your decision.

## Test Dependencies:
- Python package `lit` (because the test infrastructure uses Lit testing infrastructure).
- `gtest` in `vcpkg` for unit tests.
- `benchmark` in `vcpkg` for benchmarks.
- `gcov` and `gcovr` for code coverage (has to be found in **PATH**).
- NOTE: **Ninja** won't work for code coverage due to a bug, so a workaround is to use **Ninja Multi-Config**.

## Using Built-in Tests:
- Specify `MATRIXPP_BUILD_TESTS` to a boolean value to enable or disable building the test.
    - This enables all of the following implicitly:
        - `MATRIXPP_BUILD_COMPILE_TESTS` to `TRUE`.
        - `MATRIXPP_BUILD_UNIT_TESTS` to `TRUE`.
    - You can manually specify one of those above options to `FALSE` should you disable that specific test.
- Specify `MATRIXPP_BUILD_BENCHMARKS` to a boolean value to enable or disable building the benchmarks.
    - Recommended to build in **Release** configuration.

## Using the Lit Test Suite:
- Running `ctest` in `<build directory>` directory invokes all tests (use `ctest -V` for verbose output).
- Running `lit` either in `<build directory>/bin/tests/unit_tests` or `<build directory>/bin/tests/compile_tests` runs all the test in that directory.
- You can manually run specific unit tests by invoking the executable (don't double click on them, instead run them from command line).
- You can manually run specific compile tests by using Lit's parameter `--filter` and pass a regex string to match the tests.

## Using the Performance Test Suite:
- Each benchmarks will be comppiled to an executable, and all the benchmarks will be located in `<build directory>/bin/benchmarks`.
- The benchmarks are written using `Google Benchmark`, which means you may use `Google Benchmark` command line options when invoking the executable.
- There will be an executable called `all_benchmarks`, which invokes all the benchmarks.

## Using Code Coverage:
- Specify `MATRIXPP_CODE_COVERAGE` to a boolean value to enable or disable code coverage.
- **NOTE**: All the requirements are found in https://github.com/bilke/cmake-modules/blob/master/CodeCoverage.cmake.
    - Recommended to build in **Debug** configuration.
    - Coverage file will be in `<build directory>` named `matrixpp_code_coverage.xml`.