# Solution notes

Stop here until you have attempted the exercises.

## C++

Latitude is `std::isfinite(latitude) && latitude >= -90.0 && latitude <= 90.0`.
For the greatest score, first choose the empty-input contract. Throwing
`std::invalid_argument` is explicit. Then use `std::max_element` and dereference
the returned iterator. The `const&` avoids copying the vector.

## Java

Use `input.stream().sorted(Comparator.comparingDouble(Result::score).reversed())
.toList()`. `toList()` produces a new unmodifiable list, leaving input alone. A
copied mutable `ArrayList` plus `sort` is also valid.

## Python

Track a `seen` set. For each target, test membership before adding. A compact
length comparison also detects duplication, but a loop can report the exact ID
and teaches the algorithm more clearly.
