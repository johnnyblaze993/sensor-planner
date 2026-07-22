# Debugging guide

## First classify the failure

- Compiler error: one source file could not become an object file.
- Linker error: compiled objects could not be combined; often a missing
  definition or library.
- Runtime error: the executable started and failed afterward.
- HTTP error: inspect both status and JSON error body.
- Validation failure: find which boundary rejected which field.

Read the first relevant error, not the final summary. Later errors often cascade.

## Debug C++

Build with symbols, then use GDB:

```bash
cmake -S planner-core -B planner-core/build -DCMAKE_BUILD_TYPE=Debug
cmake --build planner-core/build
gdb --args planner-core/build/planner-core --input planner-core/sample-data/scenario.json
```

Useful GDB commands: `break main`, `run`, `next`, `step`, `print variable`,
`backtrace`, and `quit`. A stack trace tells you how execution reached a fault.

## Debug the Java/native boundary

Run the executable directly with the exact JSON first. Check:

1. Does the configured path exist and have execute permission?
2. Does C++ write clean JSON to stdout?
3. Does it write explanations to stderr?
4. What exit code does it return (`echo $?`)?
5. Does it finish before the configured timeout?

## Debug Python

Use `pytest -vv`, add `-k test_name` to isolate a test, or run `python -m pdb
script.py`. Print JSON with indentation, but do not swallow the HTTP status or
response body during investigation.
