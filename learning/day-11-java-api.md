# Day 11 — The Java API and native process

## Goal

Understand child processes, pipes, timeouts, exit codes, JSON boundaries, HTTP
errors, and focused tests.

`ProcessBuilder.start()` asks the operating system to launch C++. Java writes
JSON, closes stdin to signal EOF, drains stdout/stderr concurrently, and waits
with a timeout. A child can otherwise hang or fill a pipe buffer and deadlock.

The adapter translates outcomes:

| Native outcome | Java result |
|---|---|
| Exit 0 + valid JSON | `PlanResult`, HTTP 200 |
| Exit nonzero | `PlannerExecutionException`, HTTP 502 |
| Invalid JSON | `PlannerExecutionException`, HTTP 502 |
| Timeout | forced termination, HTTP 502 |
| Executable missing | I/O failure, HTTP 502 |

The controller test uses MockMvc and a mock service, so it tests HTTP mapping
without C++. The client test uses tiny temporary executables, so it tests real
process behavior without depending on planner math. These scopes make failures
diagnosable.

## Practice

1. Add and test the timeout case using a script that sleeps two seconds.
2. Make C++ log to stdout and observe why the JSON contract breaks.
3. Add a request correlation ID conceptually; decide which layer should log it.
4. Explain why shell interpolation is not used to form the command.

Checklist: I can diagnose each failure using status, body, stderr, and exit code.
