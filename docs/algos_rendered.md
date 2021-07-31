### Algorithms

These are the algorithms currently implemented:

| Algorithm  | API |
| ------------- | ------------- |
| Block  | `mpp::block` |
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
<p align="center"><img src="https://cdn.jsdelivr.net/gh/sam20908/mpp@None/svgs/59d7d6518ebe8208a7258623a7bca342.svg?invert_in_darkmode" align=middle width=109.13243384999998pt height=39.452455349999994pt/></p>
and we want to grab this portion of the matrix (block/submatrix):
<p align="center"><img src="https://rawgit.com/sam20908/mpp/None/svgs/93c0426b71d734bfd91f447b4211f10c.svg?invert_in_darkmode" align=middle width=81.78649709999999pt height=19.726228499999998pt/></p>

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
<p align="center"><img src="https://rawgit.com/sam20908/mpp/None/svgs/4676e3b732b616b7fcd0fe55941c3d47.svg?invert_in_darkmode" align=middle width=251.8778889pt height=78.9048876pt/></p>

And you wanted to solve for `X` such that:

<p align="center"><img src="https://rawgit.com/sam20908/mpp/None/svgs/8581353a036ae19238c4412a5a5d3569.svg?invert_in_darkmode" align=middle width=61.30694954999999pt height=11.232861749999998pt/></p>

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
<p align="center"><img src="https://rawgit.com/sam20908/mpp/None/svgs/a3120de4bb26c7c06c8bb3dd635b9d97.svg?invert_in_darkmode" align=middle width=220.82982239999998pt height=59.1786591pt/></p>

And you wanted to solve for `X` such that:

<p align="center"><img src="https://rawgit.com/sam20908/mpp/None/svgs/43a4e2a8a5ed1f37e4b252136897f258.svg?invert_in_darkmode" align=middle width=63.135646200000004pt height=11.232861749999998pt/></p>

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
<p align="center"><img src="https://rawgit.com/sam20908/mpp/None/svgs/1c28ceb585aa71c2a6639e8eabec6b7b.svg?invert_in_darkmode" align=middle width=84.47487345pt height=39.452455349999994pt/></p>

You want to find the matrices `L` and `U` such that:

<p align="center"><img src="https://rawgit.com/sam20908/mpp/None/svgs/15d3a665e7c377f7c66990d1032968d1.svg?invert_in_darkmode" align=middle width=58.44963299999999pt height=11.232861749999998pt/></p>

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

<p align="center"><img src="https://rawgit.com/sam20908/mpp/None/svgs/6b2e85ac8ff3188a5ffc284fe75c851c.svg?invert_in_darkmode" align=middle width=89.04107024999999pt height=59.1786591pt/></p>

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

<p align="center"><img src="https://rawgit.com/sam20908/mpp/None/svgs/d1ff0a1df63754a1c0de1c6f75bb4105.svg?invert_in_darkmode" align=middle width=126.48405659999999pt height=59.1786591pt/></p>

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

<p align="center"><img src="https://rawgit.com/sam20908/mpp/None/svgs/a0affc8482f6244253b459fa68ef452f.svg?invert_in_darkmode" align=middle width=139.2694842pt height=59.1786591pt/></p>

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
