## :gear: Bugfixes:
- [ ] :heavy_exclamation_mark: `singular` should respect user provided precision type (it correctly only checks against `long double`).
- [ ] :heavy_exclamation_mark: `identity_matrix` can't have rank `0`.
- [ ] :heavy_exclamation_mark: `zero_matrix` can't have rank `0`.
- [ ] :heavy_exclamation_mark: `determinant` should **always** return floating point.
- [ ] :heavy_exclamation_mark: `inverse` should **always** return floating point matrix.

## :tada: Improvements:
- [ ] :heavy_exclamation_mark: Use a faster algorithm (with exchange for more space complexity) for determinant calculation.
- [ ] :heavy_exclamation_mark: Use a faster algorithm (with exchange for more space complexity) for inverse calculation.
- [ ] :small_red_triangle: Assertion for a fully static matrix that one side can't be zero and other being a non-zero should be a `static_assert`.
- [ ] :small_red_triangle: `row` should return a row vector.
- [ ] :small_red_triangle: `column` should return a column vector.
- [ ] :100: Allow passing user specified format to `print`.
- [ ] :100: Implement an iterator class so that `matrix` can be iterated as a 2D range.
- [ ] :100: `identity_matrix`'s constructor parameter should be renamed from `side_length` to `rank`.

## :mountain_railway: Tests:
- [ ] :100: **Performance benchmark**.
- [ ] :small_red_triangle_down: Add tests for `matrix`:
    ### Instantiation:
    - [ ] :x: Instantiation of a non-arithmetic type.
    ### Constructor:
    - [ ] :x: Pass a 1D or 3D+ range to the range constructor overload.
    - [ ] :heavy_check_mark: `matrix_reserve_only_tag` constructor should only reserve (does not increase the size of the buffer).
    - [ ] :heavy_check_mark: Default copy constructor should perform a deep copy.
    - [ ] :heavy_check_mark: Default move constructor should move the data into the other object and leave the original one in a zombie state.
    - [ ] :heavy_check_mark: Constructors that take dimensions as its parameter should initialize its dimension from the compile time specified size for **fully** static matrices.
    - [ ] :heavy_check_mark: Constructors that take dimensions as its parameter should initialize its dimension from the parameter specified size for **not fully** static matrices.
    - [ ] :heavy_check_mark: Buffer type should be a 1D `std::array` for **fully** static matrices.
    - [ ] :heavy_check_mark: Buffer type should be a 1D `std::vector` for **not fully** static matrices.
    - [ ] :heavy_check_mark: Passing a 2D `std::initializer_list` should give the correct dimension.
    - [ ] :x: Passing a 2D `std::initializer_list` **with uneven length of rows**.
    - [ ] :heavy_check_mark: Passing a 2D `std::array` should have the compile time specified size deduced in the template argument.
    - [ ] :x: Passing a 2D range **with uneven length of rows**.
    - [ ] :heavy_check_mark: Passing a 2D range should give the correct dimension.
    - [ ] :heavy_check_mark: Initialization from an expression template with same or bigger dimension (because it's specified at compile time).
    - [ ] :x: Initialization from an expression tepmlate with smaller compile time specified dimension.
    ### Data members:
    - [ ] :heavy_check_mark: `data` should return the internal data that is the same as the initializer.
    - [ ] :heavy_check_mark: Non-mutable `matrix` should be iterable.
    - [ ] :heavy_check_mark: Mutable `matrix` should be iterable.
    - [ ] :heavy_check_mark: `type` should return the correct `matrix_type`.
    - [ ] :heavy_check_mark: Static matrices should have their `rows` and `columns` return corresponding dimension of the compile time specified dimension.
    - [ ] :heavy_check_mark: Dynamic matrices should have their `rows` and `columns` return the current dimension.
    - [ ] :x: Accessing out of bounds using `at`.
    - [ ] :heavy_check_mark: `square` should be true for square matrices.
    - [ ] :heavy_check_mark: `determinant` value should be correct and very precise for floating points **(need a lot of test cases for this)**.
    - [ ] :heavy_check_mark: `inverse` should be correct and very precise **(need a lot of test cases for this)**.
    - [ ] :heavy_check_mark: `transpose` should be correct.
    - [ ] :x: Passing any out of bound index to `block`.
    - [ ] :x: Passing overlapped index (top index bigger than bottom index) to `block`.
    - [ ] :heavy_check_mark: `block` should return the correct block of elements.
    - [ ] :heavy_check_mark: `cast` should return a new matrix with all elements of the casted type.
    - [ ] :heavy_check_mark: `operator()` should return the correct corresponding element.
    ### Matrix mathematical operations:
    - [ ] :x: Adding two different sized matrices.
    - [ ] :heavy_check_mark: Adding two same sized matrices should give the correct matrix.
    - [ ] :x: Subtracting two different sized matrices.
    - [ ] :heavy_check_mark: Subtracting two same sized matrices should give the correct matrix.
    - [ ] :heavy_check_mark: Multiplying a matrix by a scalar should give the same size matrix.
    - [ ] :heavy_check_mark: Multiplying a matrix by a scalar should give the correct matrix.
    - [ ] :x: Multiplying two matrices where the left matrix's columns isn't the same as the right matrix's rows.
    - [ ] :heavy_check_mark: Multiplying two matrices where the left matrix's columns is the same as the right matrix's rows should give the correct matrix.
    - [ ] :heavy_check_mark: Dividing a matrix by a scalar should give the same size matrix.
    - [ ] :heavy_check_mark: Dividing a matrix by a scalar should give the correct matrix.
    - [ ] :heavy_check_mark: Binary operator chains should result in an expression template and no copies of the operands.
    ### Identity matrix
    - [ ] :heavy_check_mark: Diagnal elements should be `1`.
    - [ ] :heavy_check_mark: Non-diagnal elements should be `0`.
    - [ ] :x: Passing the rank to the constructor when the dimension is already specified at compile time.
    - [ ] :x: Default constructing when the rank isn't specified in compile time.
    - [ ] :heavy_check_mark: It should **always** be square.
    - [ ] :heavy_check_mark: Default copy constructor should perform a deep copy.
    - [ ] :heavy_check_mark: Default move constructor should move the data into the other object and leave the original one in a zombie state.
    ### Zero matrix
    - [ ] :heavy_check_mark: All elements should be `0`.
    - [ ] :heavy_check_mark: Passing the rank to the constructor when the dimension is already specified at compile time should still use the specified dimension.
    - [ ] :x: Default constructing when the rank isn't specified in compile time.
    - [ ] :heavy_check_mark: It should **always** be square.
    - [ ] :heavy_check_mark: Default copy constructor should perform a deep copy.
    - [ ] :heavy_check_mark: Default move constructor should move the data into the other object and leave the original one in a zombie state.

## :beginner: CI:
- [ ] :rocket: Use **Azure Pipelines**:
    - [ ] :small_orange_diamond: Test build on Windows.
    - [ ] :small_orange_diamond: Test build on Linux.
    - [ ] :small_orange_diamond: Test build on MacOS.
    - [ ] :mag_right: Check that the code is formatted properly with **Clang Format**.
    - [ ] :bar_chart: Report test statistics to Azure Pipelines.
    - [ ] :bar_chart: Report performance statistics to Azure Pipelines.

## :page_with_curl: Licensing:
- [x] :pencil2: Mark all sources with **Apache License 2.0** header
- [x] :pencil2: `LICENSE` file
- [x] :pencil2: `NOTICE` file

## :thinking: Considering:
- :bulb: Saturated arithmetic operations in matrix operations (a good example candidate is `determinant`, where the values can unpredictably exceed the matrix element type. Currently, having the user choose the precision type only solves half of the problem, since it's still pretty difficult to judge the precision needed).