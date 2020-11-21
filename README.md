# matrixpp
A C++20 and later matrix library

## Test Dependencies:
- `gtest` in `vcpkg`.
- Python package `lit`.

## Using the Lit Test Suite:
- Running `ctest` in `build` directory invokes all tests (use `ctest -V` for verbose output).
- Running `lit` either in `build/bin/test/unit_test` or `build/bin/test/compile_test` runs all the test in that directory.
- You can manually run specific unit tests by invoking the executable (don't double click on them, instead run them from command line).
- You can manually run specific compile tests by specifying `-D COMPILE_TESTS_TO_RUN=...` with the tests being the relative directory (separated by comma if more than one tests are specified). For example:
    - *Note: This assumes you're in `build/bin/test/compile_test` directory!*
    1. `lit . -D COMPILE_TESTS_TO_RUN=instantiate/1d_range_constructor,instantiate/3d_range_constructor`
    2. The above command runs only the two specified tests.
