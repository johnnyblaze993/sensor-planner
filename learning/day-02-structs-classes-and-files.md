# Day 2 — Structs, classes, and files

## Goal

Understand data-only structs, behavior-owning classes, headers, implementation
files, constructors, access control, and `#include`.

```cpp
struct Coordinate {          // Public members by default.
    double latitude{};       // {} value-initializes to 0.0.
    double longitude{};
};

class Counter {              // Private members by default.
public:
    explicit Counter(int start) : value_(start) {}
    void increment();
    int value() const { return value_; }
private:
    int value_;
};
```

A header declares the contract. A `.cpp` file supplies definitions. Include
guards (`#pragma once` here) prevent duplicate declarations in one translation
unit. Compilation happens per `.cpp`; linking connects calls with definitions.

## Project tour

- `Models.hpp`: small public data structures.
- `Planner.hpp`: class declaration and public operations.
- `Planner.cpp`: function definitions and hidden constants.
- `main.cpp`: executable entry point.

Notice `Planner` needs no constructor because it owns no state. C++ generates a
default constructor. Adding ceremony would obscure that useful fact.

## Practice

1. Add a data-only `Sensor { std::string id; Coordinate position; }` on paper.
2. Create `Temperature.hpp`, `Temperature.cpp`, and `main.cpp`; compile each to
   `.o` with `g++ -c`, then link them.
3. Move a private member to `public` and explain why callers can now violate an
   invariant.
4. Remove a function definition and identify the resulting linker error.

Checklist: I can say whether an error happened during compilation or linking.
