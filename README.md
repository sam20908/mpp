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

#### Overview

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

  const auto rng_2d = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };
  auto m_2d_range = mpp::matrix{ rng_2d }; // Initialize from a 2D range
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

#### Comparisons

**Note: comparisons only work for matrices of the same value type!**

Normally with C++20, we could simply provide a `operator<=>` and let the compiler figure out the approriate operators, but that's not going to work when we want to compare matrices of different extents because of the different base class types. Instead, comparison CPOs were implemented to cover that gap.

```cpp

#include <mpp/matrix.hpp>
#include <mpp/utility/comparator.hpp> // mpp::compare_three_way_equivalent
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
    const auto ordering_2 = mpp::elements_compare(left, right, mpp::compare_three_way_equivalent);
    // mpp::compare_three_way_equivalent is exposed to the public as a comparator that handles floating points
    // ordering_2 -> std::partial_ordering::equivalent

    return 0;
}
```

#### Customizations

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

#### Custom Iterators

Custom iterators are used to make navigating matrices easier. They also meet `LegacyContiguousIterator` requirement.

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

  return 0;
}
```

You can find more APIs that are not mentioned in this README in the (upcoming) documentation.

## FAQ

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
