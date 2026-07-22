package com.example.sensorplanner.service;

import com.example.sensorplanner.config.PlannerProperties;
import com.example.sensorplanner.model.Coordinate;
import com.example.sensorplanner.model.PlanRequest;
import com.example.sensorplanner.model.Target;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.attribute.PosixFilePermissions;
import java.time.Duration;
import java.util.List;

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.api.Assertions.assertThatThrownBy;

class CppPlannerClientTest {
    @TempDir Path temporaryDirectory;
    private final ObjectMapper objectMapper = new ObjectMapper();

    @Test
    void exchangesJsonWithAChildProcess() throws IOException {
        Path executable = script("success.sh", """
                #!/bin/sh
                cat >/dev/null
                printf '{"rankedTargets":[{"id":"t1","distanceKm":2.5,"score":0.8,"visible":true}]}'
                """);
        assertThat(client(executable).execute(request()).rankedTargets()).hasSize(1);
    }

    @Test
    void reportsANonZeroNativeExit() throws IOException {
        Path executable = script("failure.sh", """
                #!/bin/sh
                cat >/dev/null
                echo 'bad input' >&2
                exit 7
                """);
        assertThatThrownBy(() -> client(executable).execute(request()))
                .isInstanceOf(PlannerExecutionException.class)
                .hasMessageContaining("code 7").hasMessageContaining("bad input");
    }

    @Test
    void reportsMalformedNativeJson() throws IOException {
        Path executable = script("malformed.sh", """
                #!/bin/sh
                cat >/dev/null
                echo 'not-json'
                """);
        assertThatThrownBy(() -> client(executable).execute(request()))
                .isInstanceOf(PlannerExecutionException.class)
                .hasMessageContaining("malformed JSON");
    }

    @Test
    void stopsAPlannerThatExceedsItsTimeout() throws IOException {
        Path executable = script("timeout.sh", """
                #!/bin/sh
                cat >/dev/null
                sleep 2
                """);
        CppPlannerClient client = new CppPlannerClient(objectMapper,
                new PlannerProperties(executable.toString(), Duration.ofMillis(50)));

        assertThatThrownBy(() -> client.execute(request()))
                .isInstanceOf(PlannerExecutionException.class)
                .hasMessageContaining("timed out");
    }

    private CppPlannerClient client(Path executable) {
        return new CppPlannerClient(objectMapper,
                new PlannerProperties(executable.toString(), Duration.ofSeconds(1)));
    }

    private PlanRequest request() {
        return new PlanRequest(new Coordinate(34.7, -86.6, 180.0),
                List.of(new Target("t1", 34.8, -86.5, 200.0)));
    }

    private Path script(String name, String contents) throws IOException {
        Path path = temporaryDirectory.resolve(name);
        Files.writeString(path, contents);
        Files.setPosixFilePermissions(path, PosixFilePermissions.fromString("rwxr-xr-x"));
        return path;
    }
}
