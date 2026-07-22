# Day 7 — Spring Boot, Maven, and REST

## Goal

Understand the Maven lifecycle and implement/test one validated endpoint.

```bash
cd planner-service
mvn clean
mvn test
mvn package
mvn dependency:tree
```

Maven reads `pom.xml`. Conventional directories remove configuration:
`src/main/java`, `src/main/resources`, `src/test/java`, and generated `target/`.
Spring Boot starters provide compatible dependency sets and auto-configuration.

`@RestController` makes return values HTTP response bodies. `@PostMapping`
selects method and path. `@RequestBody` deserializes JSON. `@Valid` runs record
constraints. Success is HTTP 200, bad client input is 400, and a failed native
dependency becomes 502.

The project uses Spring Boot 3.5.7 with Java 21. That version keeps the teaching
surface stable while using modern records and Jakarta Validation.

## Project connection

Follow `POST /api/plans` through `PlanController`, `DefaultPlannerService`, and
`CppPlannerClient`. Then read `GlobalExceptionHandler` and its error JSON.

## Practice

1. Send valid JSON with curl.
2. Remove `sensor` and inspect the 400 body.
3. Point the executable property at a missing file and inspect the 502 body.
4. Add a controller test for an empty target list.

Checklist: I can package the jar and explain controller/service/client boundaries.
