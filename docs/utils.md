### Utilities

These are the algorithms currently implemented:

| Utility | API |
| ------------- | ------------- |
| Configuration | `mpp::cfg` |
| Print | `mpp::print` |
| Type | `mpp::type` |

#### Configuration

It is explained [here](customize.md)

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

#### Type

It returns the type of matrix via an enum class:

```cpp
// actual content of mat_type inside mpp
enum class mat_type
{
    fixed,
    dynamic,
    dynamic_rows,
    dynamic_cols
};

mat<int, 1, 1> a;
mat<int, dyn, dyn> b;
mat<int, dyn, 1> c;
mat<int, 1, dyn> d;

type(a); // mat_type::fixed
type(b); // mat_type::dynamic
type(c); // mat_type::dynamic_rows
type(d); // mat_type::dynamic_cols
```
