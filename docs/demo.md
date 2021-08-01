### Adding it to your project

This project uses CMake buildsystem, so you can do it via CMake:

```cmake
add_subdirectory("mpp")

target_link_libraries(your_target PUBLIC mpp::mpp)
```

Different components are split into different headers, which means we have these:

```cpp
#include <mpp/algo/block.hpp> // and more algorithm headers
#include <mpp/algo.hpp> // includes all algorithms

#include <mpp/util/type.hpp> // and more utility headers
#include <mpp/util.hpp> // includes all utilities

#include <mpp/arith/add.hpp> // and more arithmetic headers
#include <mpp/arith.hpp> // includes all arithmetics

#include <mpp/mat/fixed.hpp> // and more matrix headers
#include <mpp/mat.hpp> // includes all matrices

#include <mpp/mpp.hpp> // includes everything
```

Note that other documentations will omit the `mpp` namespace, so it is safe to assume that it does:

```cpp
using namespace mpp;
```

---

### Demo

Let's say you wanted to create this matrix:
$$
A=
\begin{bmatrix}
1 & 2 & 3\\
4 & 5 & 6
\end{bmatrix}
$$
You can do it in many ways:

```cpp
std::vector<std::vector<float>> a{
    {2, 3, 4},
    {5, 6, 7}
};

// Anything with mat<int, 2, 3>, mat<int, dyn, dyn> etc are showing extents

// 2D range (constructor can figure out dimensions of 2D range)
mat<int, 2, 3> A{a};
mat<int, dyn, dyn> A{a};
mat<int, dyn, 3> A{a};
mat<int, 2, dyn> A{a};

// 2D std::initializer_list
mat<int, 2, 3> A{
    {1, 2, 3},
    {4, 5, 6}
};
mat<int, dyn, dyn> A{
    {1, 2, 3},
    {4, 5, 6}
};
mat<int, dyn, 3> A{
    {1, 2, 3},
    {4, 5, 6}
};
mat<int, 2, dyn> A{
    {1, 2, 3},
    {4, 5, 6}
};

// 1D representation
std::array<int, 6> b{1, 2, 3, 4, 5, 6};

mat<int, 2, 3> A{b};
mat<int, dyn, dyn> A{2, 3, b};
mat<int, dyn, 3> A{2, b};
mat<int, 2, dyn> A{3, b};

// Using callable return values
auto iota = [i = 1]() mutable {
    return i++;
};
// 2 means 2 rows and 3 means 3 columns
mat<int, 2, 3> A{iota};
mat<int, dyn, dyn> A{2, 3, iota};
mat<int, dyn, 3> A{2, iota};
mat<int, 2, dyn> A{3, iota};
```

Explanation for extents are [here](extents.md)

Now, what can I do with it?

1. You can access and get information about the matrix itself:

```cpp
A.rows(); // 2
A.cols(); // 3

A.rows_extent(); // equal to Rows template parameter
A.cols_extent(); // equal to Cols template parameter

for (int val : A) { /* code */ } // Iterates as 1D range

// These iterators meet std::contiguous_iterator
auto begin = A.begin();
auto end = A.end();
auto cbegin = A.cbegin();
auto cend = A.cend();
// Also have reverse variants (rbegin, crbegin, etc)

A.size(); // 6, because 2 * 3 = 6
A.max_size(); // depends on the internal buffer used

A.front(); // 1
A.back(); // 6

A(0, 0); // 0-indexed, accesses (1, 1) of A
A[5]; // 0-indexed, accesses (2, 3) of A (avoids calculation of 2D index to 1D index)

A.empty(); // false
```

You may have noticed that most of these member functions are the member function that STL containers have that meets `Container` requirement, and that is intentional!

2. You can also mutate the matrix in a lot of ways:

```cpp
A(0, 0) = 5;
A[5] = -1;
// Applies for .front(), .back(), etc

*(A.begin()) = 2;
// Applies for all iterators

mat B{
    {2, 3, 4},
    {5, 6, 7}
};

std::vector<std::vector<float>> C{
    {2, 3, 4},
    {5, 6, 7}
};

// Anything that takes C means it also takes any 2D range

A = B;
A = C;
A = std::move(B);
A = std::move(C);

// Same as operator=
A.assign(B);
A.assign(C);

A.swap(B);
```

### Moving on

Only so much functionality have been put on matrices itself, while the rest of them are grouped away for easier maintainance and compile time reasons. Now, you can move on to the **real deal**.

* Learn about extra iterator functionalities [here](more_iter_funcs.md)
* Learn about algorithms [here](algos.md)
* Learn about utilities [here](utils.md)
* Learn about customizations [here](customize.md)
