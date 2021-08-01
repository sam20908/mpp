### Customizations

#### Customizing extents

The matrices default to `dyn` (dynamic) when we don't specify the extent parameters.

```cpp
mat<int> a; // mat<int, dyn, dyn>
```

We can change the default values for the extents (for now, you have to specify all the values). Here is how the default configuration looks like, so you can understand what to tweak:

```cpp
#include <mpp/util/cfg.hpp>

#include <array>
#include <vector>

// in order for the customization to take effect, it has to be specialized before any other include of mpp
namespace mpp
{
  template<>
  struct cfg<override>
  {
    template<typename Val>
    using alloc = my_custom_allocator<Val>;

    static constexpr std::size_t rows_extent = dyn;
    static constexpr std::size_t cols_extent = dyn;

    template<typename Val, std::size_t Rows, std::size_t Cols, typename>
    using fixed_buf = std::array<Val, Rows * Cols>; // used when both extents aren't dyn (fixed)

    template<typename Val, std::size_t, std::size_t, typename Alloc>
    using dyn_buf = std::vector<Val, Alloc>; // used when both extents are dyn (dynamic)

    template<typename Val, std::size_t, std::size_t Cols, typename Alloc>
    using dyn_rows_buf = dyn_buf<Val, 1, Cols, Alloc>; // used when rows extent is dyn and columns extent is not dyn (dynamic rows)

    template<typename Val, std::size_t Rows, std::size_t, typename Alloc>
    using dyn_cols_buf = dyn_buf<Val, Rows, 1, Alloc>; // used when rows extent is not dyn and columns extent is dyn (dynamic columns)
  };
}
```

For example, you want the default extents to be 10 rows and 10 columns, so you can do this:

```cpp
namespace mpp
{
  template<>
  struct cfg<override>
  {
    ...
    static constexpr std::size_t rows_extent = 10;
    static constexpr std::size_t cols_extent = 10;
    ...

mat<int> a; // mat<int, 10, 10>
```

#### Customizing CPOs

Every algorithm and utility is implemented as a CPO (customization-point object). It uses `tag_invoke` approach to reduce name clashing.

For example, you want to customize the algorithm `det` for a special form of a matrix. Let's say that you have your own type of matrix called `SingularMat`. You guarantee that the matrix is singular, so the determinant should always be `0`. Let's code that out:

```cpp
#include <mpp/algo/det.hpp>

namespace ns { // note that namespace isn't necessary
    struct SingularMat { ... };

    // we customize by algorithm closure type (mpp always prefix them with _t)
    int tag_invoke(det_t, SingularMat) {
        return 0;
    }
}

det(ns::SingularMat{}); // 0
```

There could be a lot of reasons where the library consumer (you) want to customize something for some benefit, whether that may be performance or something else. This also makes it easy for mpp to customize algorithms for special cases.
