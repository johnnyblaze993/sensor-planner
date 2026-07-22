# Curated references

Use the daily lessons first and these references when you need a second
explanation or exact API details. Reference sites are dictionaries, not books:
do not try to read them front to back.

## C++ and builds

- [C++ language reference](https://en.cppreference.com/w/cpp/language/) — dense,
  searchable details on syntax, lifetime, references, classes, and exceptions.
- [C++ Standard Library reference](https://en.cppreference.com/w/cpp/standard_library)
  — containers, algorithms, strings, streams, and smart pointers.
- [Official CMake tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
  — targets, libraries, tests, installation, and dependency concepts.
- [JSON for Modern C++](https://github.com/nlohmann/json) — the parser used by
  `planner-core`; begin with its README examples.

## Java and Spring

- [Spring Boot reference](https://docs.spring.io/spring-boot/index.html) — use
  the sections on dependency injection, web applications, validation, testing,
  configuration, Actuator, and container images.
- [Spring REST service guide](https://spring.io/guides/gs/rest-service/) — a
  smaller first endpoint before the full reference manual.
- [Maven getting-started guide](https://maven.apache.org/guides/getting-started/)
  — lifecycle, directory conventions, dependencies, and plugins.
- [Java `ProcessBuilder`](https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/ProcessBuilder.html)
  — the Java-to-C++ operating-system boundary used here.

## Python and automation

- [Official Python tutorial](https://docs.python.org/3/tutorial/) — functions,
  collections, modules, exceptions, classes, and files.
- [`dataclasses`](https://docs.python.org/3/library/dataclasses.html) and
  [`pathlib`](https://docs.python.org/3/library/pathlib.html) — the main modeling
  and file-path tools in the scripts.
- [`struct`](https://docs.python.org/3/library/struct.html) — converting between
  Python values and defined binary layouts.
- [pytest documentation](https://docs.pytest.org/) — test discovery, fixtures,
  parameterization, and failure output.

## Containers and secure delivery

- [Docker multi-stage builds](https://docs.docker.com/build/building/multi-stage/)
  — why this repository separates compiler and runtime stages.
- [Dockerfile best practices](https://docs.docker.com/build/building/best-practices/)
  — cache use, small build contexts, versioning, and non-root runtime choices.
- [GitHub Actions documentation](https://docs.github.com/actions) — understand
  each line of `.github/workflows/ci.yml` before extending it.

## Domain study

Start with `geospatial-and-radar-glossary.md`, then look for employer-approved
material on coordinate reference systems, DTED, radar fundamentals, and sensor
planning. Domain formulas depend on assumptions and data classification; never
substitute this sandbox's heuristic for an authoritative model.
