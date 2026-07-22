# Day 3 — References, pointers, and lifetime

## Goal

Read value, reference, pointer, `const`, stack/heap, smart-pointer, and RAII
syntax without treating them as mysterious punctuation.

```cpp
void copyTarget(Target target);            // Makes a copy.
void renameTarget(Target& target);          // Borrows and may modify.
double score(const Target& target);         // Borrows; cannot modify.
void maybeUse(const Target* target);        // Pointer may be nullptr.
```

A reference must refer to an object. A pointer stores an address and can be
null. Neither automatically owns the object. Never return a pointer/reference
to a local variable: that local dies at scope exit.

```cpp
auto owned = std::make_unique<Target>();    // One owner; automatic cleanup.
auto shared = std::make_shared<Target>();   // Reference-counted shared ownership.
```

Prefer values and containers first, `std::unique_ptr` for necessary dynamic
ownership, and `std::shared_ptr` only for genuinely shared lifetime. Avoid raw
`new`/`delete` in application code.

RAII means resource acquisition is tied to object lifetime. `std::vector`,
`std::string`, file streams, and smart pointers clean up in destructors even
when exceptions leave a scope.

## Project connection

Find every `const Coordinate&` in `Planner.hpp`. Find `std::ifstream` in
`Input.cpp`: when the function exits, its destructor closes the file.

## Practice

1. Draw scopes and destruction order for three local objects.
2. Write a function taking `const std::vector<Target>&`.
3. Pass `nullptr` to a pointer function and safely guard it.
4. Explain why `PlanResult` needs no manual destructor.

Checklist: I can distinguish ownership from access and explain `const T&`.
