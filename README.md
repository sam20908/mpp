# mpp

A C++20 and later matrix library

[![Build Status](https://dev.azure.com/samestimable2016/mpp/_apis/build/status/sam20908.mpp?branchName=main)](https://dev.azure.com/samestimable2016/mpp/_build/latest?definitionId=3&branchName=main)

#### Tested Compilers:

* GCC 10
* VS 16.10 (works up to VS 16.11.2, see https://github.com/microsoft/STL/issues/1814#issuecomment-845572895)

## Table of contents

#### Just getting started? Go [here](docs/demo.md)!

When you know the basics of how the library works, you can move on to these:

* Learn about extra iterator functionalities [here](docs/more_iter_funcs.md)
* Learn about algorithms [algorithms](docs/algos.md)
* Learn about utilities [here](docs/utils.md)
* Learn about customizations [here](docs/customize.md)

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

You can find more APIs that are not mentioned in this README in the (upcoming) documentation.

---

## FAQ

#### Why assertions instead of exceptions?

The operations have mathematical preconditions, which means it's safe to assume that the user knows what they're doing, but assertions can still be triggers in debug mode.
