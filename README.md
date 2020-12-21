# matrixpp
A C++20 and later matrix library

![](https://github.com/sam20908/matrixpp/workflows/CI/badge.svg)

## Including the Project:
- A target `matrixpp` will be created (or `matrixpp::matrixpp` if you prefer this form). This means you can simply `add_subdirectory` the project and link the target.
- You can also just include the header into your project manually if that's your decision.
- Specify `MATRIXPP_BUILD_TEST` to a boolean value to enable or disable building the test.
    - This enables all of the following implicitly:
        - `MATRIXPP_BUILD_COMPILE_TEST` to `TRUE`.
        - `MATRIXPP_BUILD_UNIT_TEST` to `TRUE`.
        - `MATRIXPP_BUILD_PERFROMANCE_TEST` to `TRUE`.
    - You can manually specify one of those above options to `FALSE` should you disable that specific test.

## Test Dependencies:
- `gtest` in `vcpkg` for test.
- Python package `lit` for test.
- `benchmark` in `vcpkg` for performance test.

## Using the Lit Test Suite:
- Running `ctest` in `build` directory invokes all tests (use `ctest -V` for verbose output).
- Running `lit` either in `build/bin/test/unit_test` or `build/bin/test/compile_test` runs all the test in that directory.
- You can manually run specific unit tests by invoking the executable (don't double click on them, instead run them from command line).
- You can manually run specific compile tests by specifying `-D COMPILE_TESTS_TO_RUN=...` with the tests being the relative directory to the compile test folder (separated by comma if more than one tests are specified). For example:
    - *Note: This assumes you're in `build/bin/test/compile_test` directory!*
    1. `lit . -D COMPILE_TESTS_TO_RUN=instantiate/1d_range_constructor,instantiate/3d_range_constructor`
    2. The above command runs only the two specified tests.
    - You can also specify the parent directory of subdirectories of tests, for example:
    1. `lit . -D COMPILE_TESTS_TO_RUN=instantiate`
    2. The above command runs all the tests inside `instantiate`.