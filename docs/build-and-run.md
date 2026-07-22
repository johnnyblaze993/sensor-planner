# Build and run reference

## C++ lifecycle

```bash
cmake -S planner-core -B planner-core/build -DCMAKE_BUILD_TYPE=Debug
cmake --build planner-core/build --parallel
ctest --test-dir planner-core/build --output-on-failure
```

Configure generates build files. Build compiles source into object files and
links those objects into executables. CTest runs tests registered by CMake.
Delete only `planner-core/build/` when you intentionally want a clean generated
build; source files live elsewhere.

## Java lifecycle

```bash
mvn -f planner-service/pom.xml clean
mvn -f planner-service/pom.xml test
mvn -f planner-service/pom.xml package
java -jar planner-service/target/planner-service-1.0.0-SNAPSHOT.jar
```

`clean` removes Maven output, `test` compiles and tests, and `package` creates
the executable jar. Set `PLANNER_CPP_EXECUTABLE` when the binary is not at the
default relative location.

## Python lifecycle

```bash
cd scripts
python3 -m venv .venv
source .venv/bin/activate
python -m pip install -r requirements-dev.txt
pytest -q
```

A virtual environment prevents project packages from modifying the system
Python installation. Leave it with `deactivate`.

## End-to-end terminal layout

- Terminal 1: build C++, then run Spring Boot from `planner-service/`.
- Terminal 2: run `curl`, generate scenarios, and execute the Python harness.
- Terminal 3: optionally watch logs or use a debugger.

Paths are resolved relative to the Java process's working directory, which is
why the documented local run begins inside `planner-service/`.
