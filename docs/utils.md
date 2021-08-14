### Utilities

These are the algorithms currently implemented:

| Utility | API |
| ------------- | ------------- |
| Print | `mpp::print` |
| Compare | `mpp::cmp` |

#### Print

It is very self-explanatory: it prints the matrix you give it:

```cpp
mat a{
    {1, 2},
    {3, 4}
};
mat b;

print(a); // note that this does not append newline after the last row
/**
 * 1 2
 * 3 4
 */

print(b);
/**
 * [empty matrix]
 */
```

#### Compare

This CPO was provided as a convenient helper to compare matrices. It returns a **compare-three-way** result (see https://en.cppreference.com/w/cpp/utility/compare/compare_three_way for relevant information). A helper `mpp::cmp_fp` is also provided to compare floating points.

```cpp
mat a{
    {1.0, 2.0},
    {3.0, 4.0}
};

mat b{
    {1.0, 2.0},
    {3.0, 4.1}
};

mat c{
    {1, 2},
    {3, 4}
};

mat d{
    {1, 2},
    {3, 4}
};

cmp(a, b, cmp_fp); // std::partial_ordering::less
cmp(c, d); // std::strong_ordering::equal (comparator is defaulted to `std::compare_three_way`, which doesn't handling floating points)

// Use `mpp::cmp_fn`to handle floating points and integral values. This comparator avoids the extra logic necessary for floating points when given integral values
cmp(a, b, cmp_fn); // std::partial_ordering::less
cmp(c, d, cmp_fn); // std::strong_ordering::equal
```
