# mpp

A C++20 and later matrix library

[![Build Status](https://dev.azure.com/samestimable2016/mpp/_apis/build/status/sam20908.mpp?branchName=main)](https://dev.azure.com/samestimable2016/mpp/_build/latest?definitionId=3&branchName=main)

#### Tested Compilers:

* GCC 10
* VS 16.10 (works up to VS 16.11.2, see https://github.com/microsoft/STL/issues/1814#issuecomment-845572895)

## Table of contents

#### Just getting started? Go [here](docs/super_demo.md)!

##### When you know the basics of how the library works, you can move on to these:

* Learn about extra iterator functionalities [here](more_iter_funcs.md)
* Learn about algorithms [algorithms](algos.md)
* Learn about utilities [here](utils.md)
* Learn about customizations [here](customize.md)

Here is a _super_ broken down example that showcases the API and functionality mpp offers:

```cpp
#include <mpp/mpp.hpp>

#include <iostream>

int main()
{
  /**
   * Creating matrices
   *
   * Extents determine if a matrix is fixed or (partially) flexible
   *
   * The following conditions will make the matrix fixed:
   * - Explicitly passing sizes different than mpp::dyn to the template parameters
   * - Deduction guides with 2D std::array will deduce the extents, e.g.:
   *
   * - std::array<std::array<int, 3>, 2> arr_2d{ { 1, 2, 3 }, { 4, 5, 6 } }
   * - mpp::mat{ arr_2d }; // Deduces to mpp::mat<int, 2, 3>
   */
  auto m_fixed = mpp::mat<int, 3, 3>{};
  auto m_dynamic = mpp::mat<int, mpp::dyn, mpp::dyn>{};
  auto m_dynamic_rows = mpp::mat<int, mpp::dyn, 3>{};
  auto m_dynamic_cols = mpp::mat<int, 3, mpp::dyn>{};

  // Initialize using 2D initializer list
  auto m_init_2d_list = mpp::mat{ { 1, 2, 3 }, { 4, 5, 6 } };
  // Note that deduction guides is used again, but it's deduced as mpp::mat<int, mpp::dyn, mpp::dyn>
  m_init_2d_list.rows(); // 2
  m_init_2d_list.cols(); // 3

  const auto range_2d = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };
  auto m_2d_range = mpp::mat{ range_2d }; // Initialize from a 2D range
  m_2d_range.rows(); // 2
  m_2d_range.cols(); // 3

  auto iota = [i = 0] mutable { return i++; };
  auto m_generated = mpp::mat<int, 2, 3>{ iota }; // Generates values from callable
  // Elements are: 0, 1, 2, 3, 4, 5

  /**
   * Algorithms (note: you can change output matrix type by passing a std::type_identity with desired matrix type as the last argument)
   */
  auto det = mpp::det(m_fixed);
  auto inv = mpp::inv(m_dynamic);

  auto block_fixed = mpp::block(m_fixed, 0, 0, 1, 1, std::type_identity<mpp::mat<int, 2, 2>>{});
  // mpp::mat<int, 2, 2> 2x2
  auto block_dyn = mpp::block(m_fixed, 0, 0, 1, 1);
  // mpp::mat<int, mpp::dyn, mpp::dyn> 2x2

  // LU Decomposition algorithm has the exception where you can customize the matrix type of L and U matrix
  auto test = mpp::mat<int, 3, 3>{ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
  auto [l_matrix, u_matrix] = mpp::lu(test, std::type_identity<mpp::mat<int, 3, 3>>{}, std::type_identity<mpp::mat<float>>{});
  // l_matrix is mpp::mat<int, 3, 3> and u_matrix is mpp::mat<float, mpp::dyn, mpp::dyn>

  /**
   * Utilities
   */
  auto type = mpp::type(m_fixed); // mpp::mat_type::fixed
  auto singular = mpp::sg(m_fixed); // true (because matrices default initialize elements to 0, and matrices with 0's are singular)

  /**
   * Lazy evaluated math expressions
   */
  auto expr_object = m_fixed * m_fixed * 2 / 3;
  auto result_at_top_left = expr_object(0, 0);
  auto result = mpp::mat{ expr_object }; // Force evaluation of entire matrix

  /**
   * Ways of printing matrices
   */
  mpp::print(result);
  std::cout << result; // It automatically inserts newline

  return 0;
}
```

One of the main things to take away is the concept of **"extents"** for dimensions. Matrices default to "dynamic extents", which means the sizes can be provided at runtime and matrices can be (partially) flexible (e.g. you can resize rows and cols of a "fully dynamic" matrix, but a "dynamic rows" matrix can only be resized in rows).

#### Comparisons

```cpp
#include <mpp/mpp.hpp>

int main()
{
  // Size for both of these dynamic matrices are deduced as 2x2
  const auto left = mpp::mat<int>{ { 1, 2 }, { 3, 4 } };
  const auto right = mpp::mat<int>{ { 1, 2 }, { 3, 5 } };

  const auto ordering = left <=> right; // std::strong_ordering::less

  /**
   * Comparing dimensions
   */

  const auto [row_order_1, column_order_1] = mpp::size_compare(left, right, true, true); // Compare both rows and cols
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

  const auto ordering_1 = mpp::cmp(left, right);
  // ordering_1 -> std::strong_ordering::less

  // 41.F / 99.F is recurring 0.41
  const auto left = mpp::mat<float>{ { 41.F / 99.F } };
  const auto right = mpp::mat<float>{ { 41.F / 99.F } };
  const auto ordering_2 = mpp::cmp(left, right, mpp::cmp_fp);
  // mpp::floating_point_compare is exposed to the public as a comparator that handles floating points
  // ordering_2 -> std::partial_ordering::equivalent

  return 0;
}
```

#### Customizations

Customizations of options that affect the library globally can be changed via specializing the `mpp::cfg` struct with `mpp::override` tag. **The only catch is that you have to do it BEFORE including other `mpp` headers.**

```cpp
#include <mpp/util/cfg.hpp>

#include <array>
#include <vector>

namespace mpp
{
  template<>
  struct cfg<override>
  {
    template<typename Val>
    using alloc = my_custom_allocator<Val>;

    static constexpr std::size_t rows_extent = 10;
    static constexpr std::size_t cols_extent = 10;

    /**
     * These are the default buffer type aliases the library will use, but you can customize them
     */

    template<typename Val, std::size_t Rows, std::size_t Cols, typename>
    using fixed_buf = std::array<Val, Rows * Cols>; // mpp::mat<int, 1, 2>

    template<typename Val, std::size_t, std::size_t, typename Alloc>
    using dyn_buf = std::vector<Val, Alloc>; // mpp::mat<int>

    template<typename Val, std::size_t, std::size_t Cols, typename Alloc>
    using dyn_rows_buf = dyn_buf<Val, 1, Cols, Alloc>; // mpp::mat<int, mpp::dyn, 2>

    template<typename Val, std::size_t Rows, std::size_t, typename Alloc>
    using dyn_cols_buf = dyn_buf<Val, Rows, 1, Alloc>; // mpp::mat<int, 1, mpp::dyn>
  };
} // namespace mpp

#include <mpp/mpp.hpp>

int main()
{
  auto m = mpp::mat<int>{};
  auto r = m.rows_extent(); // 10
  auto c = m.cols_extent(); // 10

  using alloc = typename mpp::mat<int, mpp::dyn, mpp::dyn>::allocator_type;
  // ^ my_custom_allocator<int>

  return 0;
}
```

Finally, note that **all algorithms and utilities** are _customization point objects_. It means that you can customize them by overloading with `tag_invoke` and it will detect your customization.

```cpp
namespace ns
{
  struct dumb_matrix {};

  [[nodiscard]] auto tag_invoke(mpp::det_t, dumb_matrix) -> int
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
  auto det = mpp::det(dummy); // 2000

  return 0;
}
```

Learn more about the rationale of using `tag_invoke` in FAQ.

You can find more APIs that are not mentioned in this README in the (upcoming) documentation.

---

## FAQ

#### Why `tag_invoke` for customization?

`tag_invoke` allows putting all the overloads into a single name, `tag_invoke` . With customization by overload resolution, name clashes are reduced down to bare minimum.

#### Why assertions instead of exceptions?

The operations have mathematical preconditions, which means it's safe to assume that the user knows what they're doing, but assertions can still be triggers in debug mode.
