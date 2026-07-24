# Sensor Planning Sandbox: Project Guidance

## Read this first

This file is the durable context for anyone assisting with this repository.
Also read these files before making a substantial change:

1. `README.md` for the project overview and quick-start commands.
2. `docs/architecture.md` for the cross-language request flow.
3. `docs/build-and-run.md` for the local build and test lifecycle.
4. The relevant lesson in `learning/` when a change is part of the curriculum.

Keep this file current when the project's purpose, architecture, contracts, or
supported toolchain changes.

## Owner and learning context

The owner is new to embedded development and is using this application to
learn:

- modern C++ and native compilation;
- Java and Spring Boot service design;
- Python automation and validation;
- how independently built programs communicate;
- testing, debugging, containers, and CI;
- concepts that transfer toward embedded development.

Learning is the primary product. Prefer small, readable, well-commented
changes that the owner can explain over clever abstractions or production-scale
complexity. When answering questions or implementing work:

- explain unfamiliar terms in plain language;
- connect the change to the C++, Java, Python, or embedded concept it teaches;
- state which process owns each piece of data and where validation occurs;
- show the relevant build/test command;
- distinguish generated files from source files;
- call out assumptions and tradeoffs rather than silently hiding them;
- preserve opportunities for the owner to practice, especially in
  `exercises/`.

Do not assume prior knowledge of pointers, object lifetime, linking, process
I/O, dependency injection, HTTP serialization, Python environments, or
cross-compilation.

## Project scope

This is a two-week, learning-oriented sensor-planning sandbox. It accepts one
synthetic sensor position and candidate targets, then returns the targets
ranked by a deliberately simple score.

All coordinates and scenarios are invented. The software is **not** an
operational radar, targeting, navigation, safety, or mission-planning system.
Do not introduce real operational or sensitive data, and do not describe the
model as physically authoritative.

The current bridge toward embedded development is native code, explicit data
formats, process boundaries, deterministic behavior, binary-record practice,
and debugging. The repository does not currently communicate with hardware,
run on a microcontroller, use an RTOS, cross-compile, or meet real-time
constraints. Be explicit about that distinction.

## Architecture and language ownership

The intentional end-to-end path is:

```text
Python scenario generator and result validator
                    |
                    | HTTP JSON: POST /api/plans
                    v
Java 21 Spring Boot validation and REST adapter
                    |
                    | child process, JSON over stdin/stdout
                    v
C++17 parsing, distance, visibility, scoring, and ranking
```

- **C++ (`planner-core/`)** owns the native calculation engine, JSON/CSV/XML
  input parsing, validation at the native boundary, ranking, and JSON output.
- **Java (`planner-service/`)** owns the HTTP API, Jakarta request validation,
  application service boundary, native-process lifecycle, timeout, and mapping
  native failures to HTTP responses.
- **Python (`scripts/`)** owns repeatable synthetic-data generation, API calls,
  property-based result checks, batch execution, and the binary-record lesson.
- **Docker/Compose** package the C++ executable and Java service together.
- **GitHub Actions** build and test each language separately, then build the
  container after those jobs pass.

The separate process boundary is intentional. Do not replace it with JNI,
rewrite all logic in one language, add a database, or introduce a message
broker unless the owner explicitly chooses that as a new learning phase.

## Important source map

```text
planner-core/
  include/planner/Models.hpp   C++ request/result value types
  include/planner/Planner.hpp  public calculation contract
  src/Input.cpp                JSON, CSV, and restricted XML parsing
  src/Planner.cpp              validation, math, scoring, and sorting
  src/main.cpp                 CLI and stdin/stdout process contract
  tests/PlannerTests.cpp       native behavior tests

planner-service/
  .../api/PlanController.java       HTTP endpoints
  .../api/GlobalExceptionHandler.java
  .../model/                       Java wire models and validation
  .../service/CppPlannerClient.java child-process adapter
  .../service/DefaultPlannerService.java
  src/main/resources/application.yml

scripts/
  scenario_models.py          Python input models
  generate_scenarios.py       deterministic synthetic inputs
  call_planner_service.py     HTTP client
  validate_results.py         response invariant checks
  run_scenarios.py            end-to-end batch harness
  binary_record_example.py    binary encoding/decoding lesson
  tests/                      Python tests

docs/                         architecture and operating references
learning/                     ordered 14-day curriculum
exercises/                    learner exercises and separate solutions
```

Directories such as `planner-core/build/`, `planner-service/target/`,
`.pytest_cache/`, `__pycache__/`, and `scripts/.venv/` are generated or local
environment state. Do not treat them as source or hand-edit their contents.

## Cross-language contract

The normal service request is JSON shaped like:

```json
{
  "sensor": {
    "latitude": 34.725,
    "longitude": -86.65,
    "altitudeMeters": 180.0
  },
  "targets": [
    {
      "id": "target-1",
      "latitude": 34.8,
      "longitude": -86.5,
      "altitudeMeters": 220.0
    }
  ]
}
```

The result is JSON shaped like:

```json
{
  "rankedTargets": [
    {
      "id": "target-1",
      "distanceKm": 16.0,
      "score": 0.75,
      "visible": true
    }
  ]
}
```

Important invariants:

- latitude is finite and in `[-90, 90]`;
- longitude is finite and in `[-180, 180]`;
- altitude is finite and non-negative;
- at least one target is required;
- target IDs are nonblank and unique;
- each input target appears exactly once in the output;
- distances are finite and non-negative;
- scores are finite, in `[0, 1]`, and sorted descending;
- equal scores use target ID as the deterministic tie-breaker.

Java and C++ both validate input because either boundary can be called
directly. Python validates response properties without duplicating the C++
formula.

The native process contract must stay clean:

- stdin receives one complete JSON request when no CLI arguments are supplied;
- stdout contains only a successful JSON result;
- diagnostics go to stderr;
- exit `0` means success, exit `1` means input/planning failure, and exit `2`
  means incorrect CLI usage;
- Java closes the child's stdin, drains stdout and stderr concurrently, and
  enforces the configured timeout.

If a wire field or behavior changes, update all affected C++, Java, Python,
sample data, tests, and documentation in the same change.

## Deliberately simplified behavior

The C++ core uses Haversine surface distance and a simplified radio-horizon
check. Its readable teaching score is:

```text
score = 0.60 * distanceScore
      + 0.20 * altitudeScore
      + 0.20 * visibilityScore
```

This ignores terrain, weather, coordinate reference systems, antenna
properties, propagation effects, and other real sensor-domain concerns. The
CSV reader does not support quoted commas, and the XML reader supports only the
documented synthetic shape. Starting one C++ process per HTTP request is also
an intentional, understandable first design.

Treat these as documented educational boundaries, not accidental claims of
production readiness.

## Supported toolchain

- C++17 with GCC or Clang
- CMake 3.24 or newer
- Java 21
- Maven 3.9 or newer
- Python 3.11 or newer
- Docker with Compose (optional)

`nlohmann/json` is pinned and fetched by CMake during the first configure.
Avoid adding dependencies when a small standard-library example teaches the
same concept more clearly.

## Build and verification

Run commands from the repository root unless a command says otherwise.

```bash
# C++
cmake -S planner-core -B planner-core/build -DCMAKE_BUILD_TYPE=Debug
cmake --build planner-core/build --parallel
ctest --test-dir planner-core/build --output-on-failure

# Java
mvn -f planner-service/pom.xml test

# Python
cd scripts
python -m pytest -q
cd ..

# Container, when relevant
docker compose build
```

For an end-to-end local run, first build C++, then start Java from
`planner-service/` so the default relative native-executable path resolves.
Generate scenarios and run the Python harness from `scripts/`. See
`docs/build-and-run.md` for exact commands.

Verification should match the scope of a change:

- C++ math/parser/model changes: run C++ tests.
- Java API/process changes: run Java tests; build C++ too for integration work.
- Python generator/validator changes: run Python tests.
- Contract changes: run all three suites and an end-to-end request.
- Packaging changes: run all suites and build the container.

Do not report success based only on compilation when behavioral tests apply.

## Change guardrails

- Preserve the clear language ownership unless a lesson intentionally explores
  another design.
- Keep source examples approachable and avoid premature frameworks or generic
  abstractions.
- Add or update tests with behavioral changes.
- Never put diagnostic logging on the C++ program's stdout.
- Preserve deterministic scenario generation and ranking.
- Prefer safe modern C++ ownership (values, RAII, standard containers, and
  smart pointers when ownership truly requires them); do not introduce raw
  `new`/`delete` merely as a teaching shortcut.
- Validate untrusted data at each independently callable boundary.
- Keep secrets, credentials, real sensor data, and sensitive coordinates out
  of source, samples, logs, and generated results.
- Do not modify exercise solutions when the task is to give the owner practice
  unless the owner explicitly asks for the solution.
- When adding a genuine embedded target later, document the board,
  architecture, toolchain, flashing method, memory constraints, interfaces,
  and hardware/simulator test procedure here.

## Documentation maintenance

When completing a meaningful feature, update the smallest relevant durable
document:

- this file for scope, architecture, working conventions, or agent guidance;
- `README.md` for the public overview and quick start;
- `docs/architecture.md` for component relationships and data flow;
- `docs/build-and-run.md` for commands and prerequisites;
- `docs/troubleshooting.md` for recurring failures;
- `learning/` for curriculum changes.

Future sessions should use this file as orientation, then inspect the current
source and Git status before assuming the repository has not changed.
