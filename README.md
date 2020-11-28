# matrixpp
A C++20 and later matrix library

## Including the Project:
- A target `matrixpp` will be created (or `matrixpp::matrixpp` if you prefer this form). This means you can simply `add_subdirectory` the project and link the target.
- YOu can also just include the header into your project manually if that's your decision.
- **Note: specify `MATRIXPP_BUILD_TEST` to a boolean value to enable or disable building the test!**

## Test Dependencies:
- `gtest` in `vcpkg`.
- Python package `lit`.

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