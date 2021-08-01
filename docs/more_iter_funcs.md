### Extra iterator functionalities

```cpp
mat<int> m{{1, 2}, {3, 4}};
auto it = m.begin(); // Points to 1

// Traverse rows
it.advance_fwd_rows(1); // Points to 3
it.advance_back_rows(1); // Points to 1

// Traverse by movement
it += {1, 1}; // Move 1 row 1 column, points to 4
it -= {0, 1}; // Move back 0 rows 1 column, points to 3

auto new_it = it + std::pair{0, 0}; // Applies for -
```
