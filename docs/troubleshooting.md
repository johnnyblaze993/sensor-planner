# Troubleshooting

## `cmake: command not found`

Install CMake 3.24 or newer and reopen the terminal. Verify with `cmake --version`.

## CMake cannot download JSON

The first configure needs access to the pinned GitHub release. Check network,
proxy, and certificate settings. The SHA-256 check prevents a changed archive
from being used silently.

## Java says the native program cannot be started

Build C++, check `ls -l planner-core/build/planner-core`, and set an absolute
`PLANNER_CPP_EXECUTABLE`. On Linux, confirm the executable bit is present.

## Maven reports `release version 21 not supported`

`java -version` and `mvn -version` may be using different JDKs. Confirm Maven
reports Java 21. If needed on Ubuntu, run
`export JAVA_HOME=/usr/lib/jvm/java-21-openjdk-amd64` in that terminal and retry.

## HTTP 502

Spring reached its native adapter but the C++ process failed, timed out, or
returned malformed JSON. Run C++ directly and inspect stderr.

## Python cannot import local modules

Run commands from `scripts/`, or set `PYTHONPATH=scripts` when invoking pytest
from the repository root.

## Port 8080 is occupied

Set `SERVER_PORT=8081` for Java and pass `--url http://localhost:8081` to Python.
