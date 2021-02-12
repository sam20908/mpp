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

## Actually Using It!

Here is a _super_ broken down example that showcases the API and functionality mpp offers:

```cpp
#include <mpp/matrix.h>
#include <mpp/algorithms.hpp>
#include <mpp/utilities.hpp>

int main()
{
  /**
   * Creating matrices
   *
   * Extents determine if a matrix is fixed or (partially) flexible
   */
  auto m_fully_static = mpp::matrix<int, 3, 3>{};
  auto m_fully_dynamic = mpp::matrix<int, std::dynamic_extent, std::dynamic_extent>{};
  auto m_dynamic_rows = mpp::matrix<int, std::dynamic_extent, 3>{};
  auto m_dynamic_columns = mpp::matrix<int, 3, std::dynamic_extent>{};

  // Initialize using 2D initializer list
  auto m_init_2d_list = mpp::matrix{ { 1, 2, 3 }, { 4, 5, 6 } };
  m_init_2d_list.rows(); // 2
  m_init_2d_list.columns(); // 3

  /**
   * Algorithms
   */
  auto det = mpp::determinant(m_fully_static);
  auto inv = mpp::inverse(m_fully_dynamic);

  /**
   * Utilities
   */
  auto type = mpp::type(m_fully_static); // mpp::matrix_type::fully_static
  auto singular = mpp::singular(m_fully_static); // true (because matrices default initialize elements)

  /**
   * Lazy evaluated math expressions
   */
  auto expr_object = m_fully_static * m_fully_static * 2 / 3;
  auto result_at_top_left = expr_object(0, 0);
  auto result = mpp::matrix{ expr_object }; // Force evaluation of entire matrix

  return 0;
}
```

One of the main things to take away is the concept of **"extents"** for dimensions. Matrices default to "dynamic extents", which means the sizes can be provided at runtime and matrices can be (partially) flexible (e.g. you can resize rows and columns of a "fully dynamic" matrix, but a "dynamic rows" matrix can only be resized in rows).

##### You can change the extent the library default to WITHOUT MACROS!

```cpp
// Import appropriate tags for customization
#include <mpp/utility/config.hpp>

namespace mpp::customize
{
  /**
   * Customizations has to take place BEFORE ANY INSTANTIATIONS!
   * (it will NOT be detected in time after).
   *
   * The namespace mpp::customize is where the user can freely open to customize.
   */

  [[nodiscard]] constexpr std::size_t tag_invoke(matrix_rows_extent_tag, customize_tag)
  {
    return 10;
  }

  [[nodiscard]] constexpr std::size_t tag_invoke(matrix_columns_extent_tag, customize_tag)
  {
    return 10;
  }
} // namespace mpp::customize

#include <mpp/matrix.hpp>

int main()
{
  auto m = mpp::matrix<int>{};
  auto r = m.rows_extent(); // 10
  auto c = m.columns_extent(); // 10

  return 0;
}
```

Finally, note that **all algorithms and utilities** are _customization point objects_. It means that you can customize them by overloading with `tag_invoke` and it will detect your customization.

```cpp
namespace ns
{
  struct dumb_matrix {};

  [[nodiscard]] auto tag_invoke(mpp::determinant_t, dumb_matrix) -> int
  {
    /**
     * Say that dumb_matrix was a "optimized" data structure and has guarantees.
     * Lets say that dumb_matrix GUARANTEES a determinant of 2000 (inpractical example).
     */
    return 2000;
  }
} // namespace ns

int main()
{
  auto dummy = ns::dumb_matrix{};
  auto det = mpp::determinant(dummy); // 2000

  return 0;
}
```

Learn more about the rationale of using `tag_invoke` in FAQ.

You can find more APIs that are not mentioned in this README in the (upcoming) documentation.

# About Built-In Suites

## Using Built-In Tests:

CMake option `MPP_BUILD_TESTS` builds all tests by default. You can toggle unit tests and compile tests via their corresponding options.

-   `MPP_BUILD_UNIT_TESTS` for unit tests.
-   `MPP_BUILD_COMPILE_TESTS` for compile tests.

It has the following dependencies:

-   `gtest` installed via `vcpkg` submodule for unit tests.
-   LLVM Lit Test Runner `lit` installed and found in **PATH**. **It is only required for **_compile tests_**!**

## Using Built-In Benchmarks:

CMake option `MPP_BUILD_BENCHMARKS` builds benchmarks that run against various algorithms.

It has the following dependencies:

-   `benchmark` installed via `vcpkg` submodule for runtime benchmarks.

## Using Code Coverage:

CMake option `MPP_CODE_COVERAGE` defines the target `mpp_code_coverage`, which will output `mpp_code_coverage.xml` in the build folder. It has the following conditions to be met.

-   The current compiler is `gcc`.
-   `gcov` and `gcovr` installed and found in **PATH**.
-   Requires unit tests to be enabled.

## Executing the Tests and Benchmarks

#### Psst: if you want the nice features of having Lit targets, then you need to have LLVM Lit installed and found in PATH!

### Unit Tests

Different categories of unit tests are built into their own executables, so it makes failing tests easier to debug. A CTest target `unit_tests` is defined, which invokes the Lit configuration of unit tests.

Example:

```
# Assuming you are in the build folder

> ctest -R unit_test

UpdateCTestConfiguration  from :/home/sames/Projects/matrixpp_actual_code/mpp/build/DartConfiguration.tcl
UpdateCTestConfiguration  from :/home/sames/Projects/matrixpp_actual_code/mpp/build/DartConfiguration.tcl
Test project /home/sames/Projects/matrixpp_actual_code/matrixpp/build
Constructing a list of tests
Done constructing a list of tests
Updating test list for fixtures
Added 0 tests to meet fixture requirements
Checking test dependency graph...
Checking test dependency graph end
test 2
    Start 2: unit_test

2: Test command: /home/sames/.local/bin/lit "."
2: Test timeout computed to be: 10000000
2: -- Testing: 68 tests, 8 workers --

# Bunch more tests here...

2: PASS: Unit Test :: /initialization_unit_test/Initialization.FullyStatic_NonZeroAndZeroSide_Throw (65 of 68)
2: PASS: Unit Test :: /initialization_unit_test/Initialization.FullyStatic_DynamicExprObject_DifferentSize_Throw (66 of 68)
2: PASS: Unit Test :: /initialization_unit_test/Initialization.FullyStatic_ZeroAndNonZeroSide_Throw (67 of 68)
2: PASS: Unit Test :: /initialization_unit_test/Initialization.FullyStatic_StaticExprObject (68 of 68)
2:
2: Testing Time: 0.10s
2:   Passed: 68
1/1 Test #2: unit_test ........................   Passed    0.23 sec

The following tests passed:
        unit_test
```

If you want to run subset of tests by regular expression, you can `cd` into `build/bin/tests/unit_tests` and pass a regular expression into `--filter=` when running `lit .`

Example:

```
# Assuming you are in "build/bin/tests/unit_tests"

> lit . --filter=Initialization

PASS: Unit Test :: /initialization_unit_test/Initialization.DynamicColumns2DInitializerList_SizeMismatch_Throw (1 of 59)
PASS: Unit Test :: /initialization_unit_test/Initialization.DymamicRows2DInitializerList_SizeMismatch_Throw (2 of 59)
PASS: Unit Test :: /initialization_unit_test/Initialization.DymamicRowsValueConstructor (3 of 59)
PASS: Unit Test :: /initialization_unit_test/Initialization.DynamicColumns2DInitializerList (4 of 59)
PASS: Unit Test :: /initialization_unit_test/Initialization.DymamicRows2DInitializerList (5 of 59)

# Bunch more initialization tests here...

PASS: Unit Test :: /initialization_unit_test/Initialization.FullyStatic_NonZeroAndZeroSide_Throw (56 of 59)
PASS: Unit Test :: /initialization_unit_test/Initialization.FullyStatic_ZeroAndNonZeroSide_Throw (57 of 59)
PASS: Unit Test :: /initialization_unit_test/Initialization.FullyStatic_DynamicExprObject_SameSize (58 of 59)
PASS: Unit Test :: /initialization_unit_test/Initialization.FullyStatic_StaticExprObject (59 of 59)

Testing Time: 0.09s
  Excluded:  9
  Passed  : 59
```

### Compile Tests

It's very much the same as unit tests, except that LLVM Lit is required to run any compile test. There is also a corresponding CTest target called `compile_tests` which just runs the compile tests through Lit.

Because it uses a separate CMakeLists to compile the tests, the Lit configuration propagates the compiler used to build mpp to also build the compile tests. This avoids issues of both mpp and the compile tests having different compilers (e.g. compile tests being "faulty" because it picked a non-conforming compiler unlike mpp).

However, the Lit configuration checks the compiler stored in the cache before it attempts to compile the tests. It will produce an error when the compilers in the cache are different than the propagated compilers (just like what CMake does).

### Benchmarks

Pretty much the same as unit tests. Different benchmarks of algorithms, and etc get built into their own executable, and a corresponding CTest target `benchmarks` invokes all benchmarks through Lit.

TODO: Add documentation once compile benchmarks are implemented!

## FAQ

#### Why LLVM Lit?

LLVM Lit Test Infrastructure helps developing test suites and benchmarks easily. It also allows combination of tests and benchmarks to be run at the same time given its flexibility. mpp's CMake scripts are also designed to integrate tests with LLVM Lit to help debug tests.

#### Why `tag_invoke` for customization?

`tag_invoke` allows putting all the overloads into a single name, `tag_invoke`, which helps the user to overload by the _type_ of the customization point objects (it is always "the name you want to overload" + `_t`), and not having to remember unique names to overload.

This was inspired from `<ranges>`, which has the customization point objects automatically perform the "two step":

```cpp
/**
 * Somewhere in the determinant implemetation WITHOUT customization point objects.
 * We have to "remember" to make an UNQUALIFIED call, only them customizations are picked up.
 */
using mpp::determinant;
determinant(obj); // Unqualified call, able to pick up overloads (customizations).
```

With `tag_invoke`, we can avoid doing `using ...`, and instead provide default implementations. Consider the following:

```cpp
struct determinant_t
{
  friend inline auto tag_invoke(determinant_t)
  {
    // Default implementation of the algorithm...
  }

  auto operator()() const
  {
    return tag_invoke(*this);
  }
};
```

The `friend` specifier makes the default `tag_invoke` in the same namespace scope as `determinant_t`, and it all works out because `tag_invoke(*this)` uses ADL to find the default overload of `tag_invoke`, and it'll also pick up other overloads because it performed an unqualified call.

#### Why store the website code in the "website" folder?

It has been painful to work between different branches for the library and the website in the same workspace. It is also NOT an optimal solution by just checking out different branches in different locations. I have decided to at least give it a dedicated folder, isolated from library code. This way, updating library code and website is a lot less painful.
