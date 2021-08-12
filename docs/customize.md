### Customizations

#### Customizing Template Parameters

The matrix class provides the following template parameters to be set:
* `T`: Value type of the matrix
* `Buf`: Internal buffer type of the matrix (default is `std::vector<T>`). Currently,  `std::vector` and `std::array` are the only buffer types supported.

For example, if you want a **fixed** matrix of size **2x3** , then you can do it like this:

```cpp
mat<int, std::array<int, 2 * 3>> // matrix stores data as 1D array
```

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
