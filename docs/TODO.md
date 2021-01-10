## :gear: Bugfixes:
- [ ] `singular` should respect user provided precision type (it correctly only checks against `long double`).
- [ ] `identity_matrix` can't have rank `0`.
- [ ] `zero_matrix` can't have rank `0`.
- [ ] `determinant` should **always** return floating point.
- [ ] `inverse` should **always** return floating point matrix.
- [x] Fully static matrices should use `static_assert` for checking one side zero and other side non-zero dimension.

## :tada: Improvements:
- [ ] Use a faster algorithm (with exchange for more space complexity) for determinant calculation.
- [ ] Use a faster algorithm (with exchange for more space complexity) for inverse calculation.
- [x] Remove duplicate determinant calculation for inverse.
- [ ] Assertion for a fully static matrix that one side can't be zero and other being a non-zero should be a `static_assert`.
- [ ] `row` should return a row vector.
- [ ] `column` should return a column vector.
- [ ] Allow passing user specified format to `print`.
- [ ] `zero` utility method.
- [ ] `random` utility method.
- [ ] Implement an iterator class so that `matrix` can be iterated as a 2D range.
- [ ] Vectorized math operations.
- [ ] Doxygen documentation for the `matrix` API.
- [x] Make matrix element type an arithmetic type.
- [x] Add missing `[[ndiscard]]`.
- [x] Add documentation about performance benchmarks in README.
- [ ] Test matrix extents in unit tests.
- [ ] Add examples in README.

## :mountain_railway: Tests:
- [x] **Performance benchmark**.
- [ ] **Memory benchmark**.
- [ ] Add unit tests.
- [ ] Add compile tests.

## :beginner: CI:
- [ ] Use **Azure Pipelines**:
    - [ ] ~~Test build on Windows (Visual Studio not up to date yet).~~
    - [x] Test build on Linux.
    - [ ] ~~Test build on MacOS (XCode not up to date yet).~~
    - [x] Check that the code is formatted properly with **Clang Format**.
    - [x] Report test statistics to Azure Pipelines.
    - [x] Report code coverage to Azure Pipelines.
    - [ ] Report performance statistics to GitHub Pages.

## :page_with_curl: Licensing:
- [x] Mark all sources with **Apache License 2.0** header
- [x] `LICENSE` file
- [x] `NOTICE` file

## :thinking: Considering: