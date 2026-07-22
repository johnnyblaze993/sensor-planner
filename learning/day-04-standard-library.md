# Day 4 — Standard Library containers and algorithms

## Goal

Use `vector`, `array`, maps, `optional`, range loops, iterators, lambdas, and
`sort` before inventing custom data structures.

```cpp
std::vector<Target> targets;
targets.push_back({"t1", {34.8, -86.5, 220.0}});

for (const Target& target : targets) {
    std::cout << target.id << '\n';
}

std::sort(targets.begin(), targets.end(),
    [](const Target& a, const Target& b) {
        return a.id < b.id;
    });
```

`begin()` and `end()` describe a half-open range. A lambda is an unnamed
function; `[]` is its capture list. `std::array<T, N>` has fixed length.
`std::map` is ordered, `std::unordered_map` is hash-based, and `std::optional<T>`
represents either one `T` or no value.

## Project connection

`Planner::createPlan` reserves vector capacity, uses a range loop, detects IDs
with `unordered_set`, and sorts with a lambda. Explain every line of that sort.

## Practice

1. Find a target ID with `std::find_if`.
2. Return `std::optional<Target>` instead of a nullable pointer.
3. Sort sample results ascending, then repair them to descending.
4. Explain why an ID set is better than nested duplicate-check loops.

Checklist: I can iterate and sort a vector without indexes when indexes add no value.
