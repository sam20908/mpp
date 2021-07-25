# mpp

A C++20 and later matrix library

[![Build Status](https://dev.azure.com/samestimable2016/mpp/_apis/build/status/sam20908.mpp?branchName=main)](https://dev.azure.com/samestimable2016/mpp/_build/latest?definitionId=3&branchName=main)

#### Tested Compilers:

* GCC 10
* VS 16.10 (works up to VS 16.11.2, see https://github.com/microsoft/STL/issues/1814#issuecomment-845572895)

## Table of contents

#### Just getting started? Go [here](docs/super_demo.md)!

When you know the basics of how the library works, you can move on to these:

* Learn about extra iterator functionalities [here](more_iter_funcs.md)
* Learn about algorithms [algorithms](algos.md)
* Learn about utilities [here](utils.md)
* Learn about customizations [here](customize.md)

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
