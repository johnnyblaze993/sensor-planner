# Architecture

## Request sequence

1. Python creates a JSON document with one sensor and one or more targets.
2. Spring MVC deserializes it into Java records.
3. Jakarta Validation rejects missing or out-of-range values with HTTP 400.
4. `DefaultPlannerService` calls the `NativePlannerClient` interface.
5. `CppPlannerClient` starts the configured native executable.
6. Java serializes the request to the process's standard input and closes it.
7. C++ parses, validates, calculates, sorts, and writes JSON to standard output.
8. Java deserializes the result and Spring writes the HTTP response.
9. Python checks the response's invariants and saves it.

## Why stdin and stdout?

They form a small language-neutral protocol. Java and C++ need agree only on
the JSON schema and exit behavior. They do not share memory, a compiler, or a
runtime. Standard output contains only successful JSON; diagnostics use
standard error. Exit `0` means success, `1` means planning/input failure, and
`2` means incorrect CLI usage.

## C++ design

`Models.hpp` owns data shapes. `Planner.hpp` is the public contract.
`Planner.cpp` hides implementation details. `Input.cpp` isolates formats from
math. `main.cpp` is deliberately thin: parse, plan, serialize, handle errors.

The `Planner` owns no resources and has no mutable state. That makes it cheap
to create and straightforward to test. Models use values and `std::vector`, so
RAII manages their memory without `new` or `delete`.

## Java design

The controller knows HTTP, the service names the use case, and the client knows
operating-system processes. The `NativePlannerClient` interface permits a fake
in a unit test. Constructor injection makes every dependency visible.

Java validates before starting C++; C++ validates again because a native CLI
can be called without Java. Validation at a trust boundary is not redundant.

## Python design

Small modules separate generation, transport, validation, and batch control.
The validator checks properties rather than copying the C++ formula. Copying
the same algorithm into tests can reproduce the same bug and give false
confidence.

## Production evolution

A production system might replace per-request processes with a persistent
worker, Unix socket, message queue, gRPC service, or carefully reviewed native
binding. Terrain data, coordinate reference systems, security controls,
observability, performance budgets, and domain validation would also be needed.
