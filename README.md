# Sensor Planning Sandbox

A two-week, learning-oriented project that connects **C++ 17**, **Java 21 with
Spring Boot**, and **Python 3**. It accepts a synthetic sensor position and
candidate targets, then returns a ranked plan.

> This project uses invented data and intentionally simplified mathematics.
> It is not an operational radar, safety, navigation, or targeting system.

## What each language owns

```text
Python scenario generator and validator
                  |
                  | HTTP POST /api/plans
                  v
Java Spring Boot validation and REST adapter
                  |
                  | JSON through stdin/stdout
                  v
C++ distance, visibility, scoring, and ranking core
```

This split is the central lesson. Language boundaries should exist for a
reason: Python is convenient for automation, Java is strong for service
boundaries, and C++ gives precise control over a native computation engine.

## Start here

1. Read [the architecture](docs/architecture.md).
2. Begin [Day 1](learning/day-01-compilation-and-syntax.md), even if you plan
   to run everything in Docker later.
3. Use [build-and-run.md](docs/build-and-run.md) as your command reference.
4. Keep [the curated references](docs/references.md) nearby for deeper reading.
5. Do the exercises before opening `exercises/solutions/`.

The `learning/` directory is ordered. Each lesson explains syntax, points to
real project files, gives commands, and ends with practice problems.

## Prerequisites

- GCC or Clang with C++17 support
- CMake 3.24+
- Java Development Kit 21
- Maven 3.9+
- Python 3.11+
- Docker with Compose (optional)

On Ubuntu:

```bash
sudo apt update
sudo apt install build-essential cmake openjdk-21-jdk maven python3-venv docker.io docker-compose-v2
```

## Build and test C++

```bash
cd planner-core
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/planner-core --input sample-data/scenario.json
./build/planner-core --input sample-data/scenario.csv
./build/planner-core --input sample-data/scenario.xml
```

`CMakeLists.txt` downloads the pinned header-only `nlohmann/json` dependency
while configuring for the first time.

## Build and test Java

Build C++ first, then:

```bash
cd planner-service
mvn clean test
mvn spring-boot:run
```

From a second terminal:

```bash
curl -s http://localhost:8080/api/health
curl -s -X POST http://localhost:8080/api/plans \
  -H 'Content-Type: application/json' \
  --data @../scripts/sample-data/scenario.json
```

The native executable can be overridden without changing code:

```bash
PLANNER_CPP_EXECUTABLE=/absolute/path/to/planner-core mvn spring-boot:run
```

## Run the Python harness

```bash
cd scripts
python3 -m venv .venv
source .venv/bin/activate
python -m pip install -r requirements-dev.txt
pytest
python generate_scenarios.py --count 10 --seed 42 --xml
python run_scenarios.py
```

Expected summary:

```text
Scenarios executed: 10
Passed: 10
Failed: 0
Average response time: ... ms
```

## Run everything in containers

```bash
docker compose up --build -d
curl -s http://localhost:8080/api/health
cd scripts && python run_scenarios.py
docker compose down
```

The image is multi-stage: it builds C++, builds Java, then copies only their
runtime artifacts into a smaller non-root runtime image.

## Scoring model

The C++ planner calculates Haversine surface distance and a simplified radio
horizon. It normalizes three values to `0..1`:

```text
score = 0.60 * distanceScore
      + 0.20 * altitudeScore
      + 0.20 * visibilityScore
```

Targets are sorted from highest score to lowest. Equal scores use the target
ID as a deterministic tie-breaker. The formula is designed to be readable and
testable, not physically authoritative.

## Useful command map

| Goal | Command |
|---|---|
| Configure C++ | `cmake -S planner-core -B planner-core/build` |
| Compile C++ | `cmake --build planner-core/build` |
| Test C++ | `ctest --test-dir planner-core/build --output-on-failure` |
| Test Java | `mvn -f planner-service/pom.xml test` |
| Package Java | `mvn -f planner-service/pom.xml package` |
| Test Python | `cd scripts && pytest` |
| Full container build | `docker compose build` |

## Intentional limitations

- The XML reader accepts only the documented synthetic XML shape. Production
  XML requires a real parser hardened against hostile input.
- CSV fields cannot contain quoted commas.
- Radio horizon ignores terrain, weather, antenna characteristics, and signal
  propagation details.
- Starting one native process per request is understandable but inefficient.
  Production alternatives include a persistent worker or a native library.
- There is no database, authentication, frontend, JNI, or real sensor data.

Those limits keep attention on repository navigation, C++ fundamentals,
service boundaries, testing, and debugging.

## GitHub

This directory is ready to initialize and publish under your account:

```bash
git init
git add .
git commit -m "Build Sensor Planning Sandbox learning project"
git branch -M main
git remote add origin https://github.com/johnnyblaze993/sensor-planner.git
git push -u origin main
```

Create the empty GitHub repository before the final two commands. Review every
file before committing; learning comes from being able to explain what you
publish.

## License

MIT. See [LICENSE](LICENSE).
