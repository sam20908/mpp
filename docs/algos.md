### Algorithms

These are the algorithms currently implemented:

| Algorithm | API |
| ------------- | ------------- |
| Block | `mpp::block` |
| Forward substitution | `mpp::fwd_sub` |
| Backward substitution | `mpp::back_sub` |
| LU decomposition | `mpp::lu` |
| Transpose | `mpp::trps` |
| Determinant | `mpp::det` |
| Inverse | `mpp::inv` |

**Algorithms do not throw exceptions**, but rather trigger assertions when encountering invalid inputs. Invalid inputs already violates mathematical <i>preconditions</i>, which means it cannot and are not responsible to handle it. Performance reasons were also why assertions were chosen.

---

#### Block/Submatrices

Let's say we have this matrix:
$$
A=
\begin{bmatrix}
1 & 2 & 3\\
4 & 5 & 6
\end{bmatrix}
$$
and we want to grab this portion of the matrix (block/submatrix):
$$
B=
\begin{bmatrix}
1 & 2
\end{bmatrix}
$$

```cpp
#include <mpp/algo/block.hpp>

mat A{
    {1, 2, 3},
    {4, 5, 6}}
;

// The 0-based indices are top_row, top_column, bottom_row, bottom_column and it is inclusive
auto B = block(a, 0, 0, 1, 0); // mat<int, dyn, dyn> by default
auto B = block(a, 0, 0, 1, 0, std::type_identity<mat<int, 1, 2>>{}); // mat<int, 1, 2>
```

---

#### Forward Substitution

Let's say you have the following:
$$
L=
\begin{bmatrix}
1 & 0 & 0 & 0\\
-1 & 1 & 0 & 0\\
0 & 0.5 & 1 & 0\\
6 & 1 & 14 & 1
\end{bmatrix}
B=
\begin{bmatrix}
1\\
-1\\
2\\
1\\
\end{bmatrix}
$$

And you wanted to solve for `X` such that:

$$
LX=B
$$

Note that permutation matrix is not supported yet

```cpp
mat<float> L{
    {1, 0, 0, 0},
    {-1, 1, 0, 0},
    {0, 0.5, 1, 0},
    {6, 1, 14, 1}
};

mat<float> B{
    {1},
    {-1},
    {2},
    {1}
};

auto X = fwd_sub(L, B); // mat<float, dyn, dyn> by default
auto X = fwd_sub(L, B, std::type_identity<mat<float, 4, 1>>{}); // mat<float, 4, 1>
```

---

#### Backward Substitution

Let's say you have the following:
$$
U=
\begin{bmatrix}
1 & 1 & 1\\
0 & -1 & 0\\
0 & 0 & -5
\end{bmatrix}
B=
\begin{bmatrix}
3\\
-4\\
4
\end{bmatrix}
$$

And you wanted to solve for `X` such that:

$$
UX=B
$$

Note that permutation matrix is not supported yet

```cpp
mat U{
    {1, 1, 1},
    {0, -1, 0},
    {0, 0, -5}
};

mat B{
    {3},
    {-4},
    {4}
};

// Answer actually has decimals despite integer inputs
auto X = back_sub(U, B); // mat<int, dyn, dyn> by default
auto X = back_sub(U, B, std::type_identity<mat<float, 3, 1>>{}); // mat<float, 3, 1>
```

---

#### LU Decomposition

Let's say you have the following:
$$
A=
\begin{bmatrix}
6 & 3\\
6 & 3
\end{bmatrix}
$$

You want to find the matrices `L` and `U` such that:

$$
A=LU
$$

```cpp
mat A{
    {6, 3},
    {6, 3}
};

// Answer actually has decimals despite integer inputs
auto [L, U] = lu(A);
/**
L=
1 0
1 1

U=
4 3
0 -1

default to same matrix type as the input (not correct in this case)
 */

using LU_t = std::type_identity<mat<float, 2, 2>>;
auto [L, U] = lu(A, std::type_identity<LU_t>{}, std::type_identity<LU_t>{}); // specify each L and U matrix type
/**
L=
1 0
1.5 1

U=
4 3
0 -1.5 (correct)
 */
```

---

#### Transpose

Let's say you want to find the transpose of this matrix:

$$
A=
\begin{bmatrix}
1 & 2\\
3 & 4\\
5 & 6
\end{bmatrix}
$$

```cpp
mat A{
    {1, 2},
    {3, 4},
    {5, 6}
};

auto ans = trps(A);
/**
1 3 5
2 4 6

default to same matrix type as input but with rows and columns flipped
 */
auto ans = trps(A, std::type_identity<mat<float, 2, 3>>{}); // Same answer as above
```

---

#### Determinant

Let's say you want to find the determinant of this matrix:

$$
A=
\begin{bmatrix}
1 & 1 & 2\\
3 & 4 & -7\\
6 & 8 & 2
\end{bmatrix}
$$

```cpp
mat A{
    {1, 1, 2},
    {3, 4, -7},
    {6, 8, 2}
};

auto ans = det(A); // 16 (default to same value type as input)
auto ans = det(A, std::type_identity<float>{}); // 16.0 (more like 15.9999998)
```

---

#### Inverse

Let's say you want to find the inverse of this matrix:

$$
A=
\begin{bmatrix}
1 & 2 & 1\\
2 & 2 & 3\\
-1 & -3 & 0
\end{bmatrix}
$$

```cpp
mat A{
    {1, 2, 1},
    {2, 2, 3},
    {-1, -3, 0}
};

auto ans = inv(A);
/**
-9 3 -4
3 -1 1
4 -1 2

default to same matrix type as the input
*/
auto ans = inv(A, std::type_identity<mat<float, 2, 3>>{}); // Same answer as above but in decimals
```
