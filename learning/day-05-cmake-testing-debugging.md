# Day 5 — CMake, testing, and debugging

## Goal

Configure, build, test, and debug the native core from a terminal.

```bash
cmake -S planner-core -B planner-core/build -DCMAKE_BUILD_TYPE=Debug
cmake --build planner-core/build
ctest --test-dir planner-core/build --output-on-failure
```

CMake defines targets and relationships. `planner_lib` compiles reusable logic;
`planner-core` and `planner-tests` link it. `PUBLIC include` means dependents
inherit the header search path. `PRIVATE` compile options affect only one target.

This project uses a tiny assertion-style test executable so you can see the
mechanism without first learning a framework. CTest cares only about exit code:
zero passes and nonzero fails. Later, try Catch2 or GoogleTest.

Debug:

```bash
gdb --args planner-core/build/planner-tests
# inside GDB
break sensor_planner::Planner::createPlan
run
next
print distanceKm
backtrace
```

## Practice

1. Add a test for longitude `181`.
2. Make it fail, read the failure, then repair the code.
3. Add a `.cpp` but omit it from `add_library`; diagnose the linker result.
4. Configure a separate `build-release` with `Release` type.

Checklist: I can interpret compiler, linker, runtime, and assertion failures.
