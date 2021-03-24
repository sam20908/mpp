# mpp (matrix++)

A C++20 and later matrix library

![Build Status](https://img.shields.io/azure-devops/build/samestimable2016/mpp/3/main?label=%F0%9F%94%A8%20Build%20Status) ![Test Results](https://img.shields.io/azure-devops/tests/samestimable2016/mpp/3/main?label=%F0%9F%A7%AA%20Test%20Results) ![Code Coverage](https://img.shields.io/azure-devops/coverage/samestimable2016/mpp/3/main?label=%F0%9F%93%B6%20Code%20Coverage) ![GitHub Pages](https://img.shields.io/github/deployments/sam20908/mpp-website/github-pages?label=%F0%9F%9A%80%20GitHub%20Pages)

#### Tested Compilers:

- GCC 10
- Clang 11
- MSVC 19.28 (VS 2019 16.9)

## How to Include:

All you need to do is just add it as a subdirectory and link the target:

```cmake
add_subdirectory("mpp")

target_link_libraries(your_target mpp::mpp)
```

---

## Overview

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
   *
   * The following conditions will make the matrix static:
   * - Explicitly passing sizes different than dynamic to the template parameters
   * - Deduction guides with 2D std::array will deduce the extents, e.g.:
   *
   * - std::array<std::array<int, 3>, 2> arr_2d{ { 1, 2, 3 }, { 4, 5, 6 } }
   * - mpp::matrix{ arr_2d }; // Deduces to mpp::matrix<int, 2, 3>
   */
  auto m_fully_static = mpp::matrix<int, 3, 3>{};
  auto m_fully_dynamic = mpp::matrix<int, dynamic, dynamic>{};
  auto m_dynamic_rows = mpp::matrix<int, dynamic, 3>{};
  auto m_dynamic_columns = mpp::matrix<int, 3, dynamic>{};

  // Initialize using 2D initializer list
  auto m_init_2d_list = mpp::matrix{ { 1, 2, 3 }, { 4, 5, 6 } };
  // Note that deduction guides is used again, but it's deduced as mpp::matrix<int, dynamic, dynamic>
  m_init_2d_list.rows(); // 2
  m_init_2d_list.columns(); // 3

  const auto range_2d = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };
  auto m_2d_range = mpp::matrix{ range_2d }; // Initialize from a 2D range
  m_2d_range.rows(); // 2
  m_2d_range.columns(); // 3

  auto iota = [i = 0] mutable { return i++; };
  auto m_generated = mpp::matrix<int, 2, 3>{ iota }; // Generates values from callable
  // Elements are: 0, 1, 2, 3, 4, 5

  /**
   * Algorithms
   */
  auto det = mpp::determinant(m_fully_static);
  auto inv = mpp::inverse(m_fully_dynamic);

  /**
   * Utilities
   */
  auto type = mpp::type(m_fully_static); // mpp::matrix_type::fully_static
  auto singular = mpp::singular(m_fully_static); // true (because matrices default initialize elements to 0, and matrices with 0's are singular)

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

#### Comparisons

**Note: comparisons only work for matrices of the same value type!**

Normally with C++20, we could simply provide a `operator<=>` and let the compiler figure out the approriate operators, but that's not going to work when we want to compare matrices of different extents because of the different base class types. Instead, comparison CPOs were implemented to cover that gap.

```cpp

#include <mpp/matrix.hpp>
#include <mpp/utility/comparison.hpp>

int main()
{
  // Note that you can't compare matrices of different value types because it complicates things A LOT

  // Size for both of these dynamic matrices are deduced as 2x2
  const auto left = mpp::matrix<int>{ { 1, 2 }, { 3, 4 } };
  const auto right = mpp::matrix<int>{ { 1, 2 }, { 3, 5 } };

  /**
   * Comparing dimensions
   */

  const auto [row_order_1, column_order_1] = mpp::size_compare(left, right, true, true); // Compare both rows and columns
  // row_order_1 -> std::partial_ordering::equivalent
  // column_order_1 -> std::partial_ordering::equivalent

  const auto [row_order_2, column_order_2] = mpp::size_compare(left, right, true, false); // Compare only rows
  // row_order_2 -> std::partial_ordering::equivalent
  // column_order_2 -> std::partial_ordering::unordered <- unordered is used to indicate "not compared"

  /**
   * Comparing elements
   *
   * Note that the ordering type may change depending on the spaceship return type for the value types
   */

  const auto ordering_1 = mpp::elements_compare(left, right);
  // ordering_1 -> std::strong_ordering::less

  // 41.F / 99.F is recurring 0.41
  const auto left = mpp::matrix<float>{ { 41.F / 99.F } };
  const auto right = mpp::matrix<float>{ { 41.F / 99.F } };
  const auto ordering_2 = mpp::elements_compare(left, right, mpp::floating_point_compare);
  // mpp::floating_point_compare is exposed to the public as a comparator that handles floating points
  // ordering_2 -> std::partial_ordering::equivalent

  return 0;
}
```

#### Customizations

Customizations of default extents used can be changed via specializing the `mpp::configuration` struct with `mpp::override` tag.

```cpp
#include <mpp/utility/configuration.hpp>

namespace mpp
{
  template<>
  struct configuration<override>
  {
    template<typename Value>
    using allocator = my_custom_allocator<Value>;

    static constexpr std::size_t rows_extent    = 10;
    static constexpr std::size_t columns_extent = 10;
  };
} // namespace mpp

#include <mpp/matrix.hpp>

int main()
{
  auto m = mpp::matrix<int>{};
  auto r = m.rows_extent(); // 10
  auto c = m.columns_extent(); // 10

  using alloc = typename mpp::matrix<int, mpp::dynamic, mpp::dynamic>::allocator_type;
  // ^ my_custom_allocator<int>, note that fully static matrices don't use allocators

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

#### Custom Iterators

Custom iterators are used to make navigating matrices easier. They also meet `contiguous_iterator` requirement.

```cpp
#include <mpp/matrix.hpp>

int main()
{
  auto matrix = mpp::matrix<int>{ { 1, 2 }, { 3, 4 } };
  auto begin = matrix.begin();

  // Extra functionalities to navigate through the matrix easier
  // 1. Nagivating by rows
  begin.move_forward_rows(1);
  auto val = *begin; // 3

  begin.move_backward_rows(1);
  ++begin;
  val = *begin; // 2

  // 2. Navigating by pair of offsets
  auto begin_2 = matrix.begin();
  begin_2 += { 1, 1 }; // 4

  begin_2 -= { 1, 0 }; // 2

  auto begin_3 = begin_2 + std::pair{ 0, 0 }; // Both point to the same location

  return 0;
}
```

You can find more APIs that are not mentioned in this README in the (upcoming) documentation.

---

## FAQ

#### Why `tag_invoke` for customization?

`tag_invoke` allows putting all the overloads into a single name, `tag_invoke`. With customization by overload resolution, name clashes are reduced down to bare minimum.
