# syntax=docker/dockerfile:1

FROM ubuntu:24.04 AS cpp-build
RUN apt-get update && apt-get install --yes --no-install-recommends \
        ca-certificates cmake g++ make \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /source/planner-core
COPY planner-core/ ./
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --parallel \
    && ctest --test-dir build --output-on-failure

FROM maven:3.9.11-eclipse-temurin-21 AS java-build
WORKDIR /source/planner-service
COPY planner-service/pom.xml ./
RUN mvn -B dependency:go-offline
COPY planner-service/src/ ./src/
RUN mvn -B package

FROM eclipse-temurin:21-jre-noble AS runtime
RUN groupadd --system planner \
    && useradd --system --gid planner --home-dir /opt/sensor-planner planner
WORKDIR /opt/sensor-planner
COPY --from=cpp-build /source/planner-core/build/planner-core ./bin/planner-core
COPY --from=java-build /source/planner-service/target/planner-service-*.jar ./planner-service.jar
RUN chown -R planner:planner /opt/sensor-planner
USER planner
ENV PLANNER_CPP_EXECUTABLE=/opt/sensor-planner/bin/planner-core
ENV PLANNER_CPP_TIMEOUT=5s
EXPOSE 8080
HEALTHCHECK --interval=10s --timeout=3s --start-period=20s --retries=3 \
  CMD curl --fail --silent http://localhost:8080/api/health >/dev/null || exit 1
ENTRYPOINT ["java", "-jar", "/opt/sensor-planner/planner-service.jar"]
