### Customizations

#### Customizing Template Parameters

The matrix class provides the following template parameters to be set:
* `T`: Value type of the matrix
* `Buf`: Internal buffer type of the matrix (default is `std::vector<T>`)

For example, if you want a **fixed** matrix of size **2x3** , then you can do it like this:

```cpp
mat<int, std::array<int, 2 * 3>> // matrix stores data as 1D array
```

If you want to provide your own buffer type, you have to "whitelist" it either with `mpp::is_dyn_buf` or `mpp::is_fixed_buf` . Using two helpers is necessary for the library to distinguish between dynamic buffers (buffers that can resize, and etc) and fixed buffers (buffers with constant size like `std::array` ).

Let's say you have your own fixed buffer:

```cpp
template<typename T>
struct my_chunk
{
    T data[4096];
    // omitted value_type, iterator, const_iterator, and etc for simplicity
};
```

You can whitelist it like this (**do this before you instantiate `mat` ):

```cpp
template<typename T>
inline constexpr auto is_fixed_buf<my_chunk<T>> = true;
```

_This was inspired by `std::ranges::enable_borrowed_range` ._

**Note that dynamic and fixed buffers are mutually exclusive. If you whitelist it as both a dynamic and fixed buffer, you'll trigger a static assertion.**

#### Customizing CPOs

Every algorithm and utility is implemented as a CPO (customization-point object). It uses `tag_invoke` approach to reduce name clashing.

For example, you want to customize the algorithm `det` for a special form of a matrix. Let's say that you have your own type of matrix called `SingularMat` . You guarantee that the matrix is singular, so the determinant should always be `0` . Let's code that out:

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
