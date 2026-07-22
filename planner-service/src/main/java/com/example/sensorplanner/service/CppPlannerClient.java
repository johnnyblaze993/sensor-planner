package com.example.sensorplanner.service;

import com.example.sensorplanner.config.PlannerProperties;
import com.example.sensorplanner.model.PlanRequest;
import com.example.sensorplanner.model.PlanResult;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.stereotype.Component;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.time.Duration;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.TimeUnit;

@Component
public class CppPlannerClient implements NativePlannerClient {
    private final ObjectMapper objectMapper;
    private final PlannerProperties properties;

    public CppPlannerClient(ObjectMapper objectMapper, PlannerProperties properties) {
        this.objectMapper = objectMapper;
        this.properties = properties;
    }

    @Override
    public PlanResult execute(PlanRequest request) {
        Process process = null;
        try {
            process = new ProcessBuilder(properties.executablePath()).start();

            // Drain both pipes concurrently. Waiting first can deadlock if a
            // child fills an operating-system pipe buffer.
            final Process runningProcess = process;
            CompletableFuture<String> stdout = CompletableFuture.supplyAsync(
                    () -> readStream(runningProcess.getInputStream()));
            CompletableFuture<String> stderr = CompletableFuture.supplyAsync(
                    () -> readStream(runningProcess.getErrorStream()));

            // Serialize first, then make ownership of the process stream clear:
            // leaving this block closes stdin and tells C++ that JSON is complete.
            byte[] requestJson = objectMapper.writeValueAsBytes(request);
            try (var processInput = process.getOutputStream()) {
                processInput.write(requestJson);
            }

            Duration timeout = properties.timeout();
            if (!process.waitFor(timeout.toMillis(), TimeUnit.MILLISECONDS)) {
                process.destroyForcibly();
                throw new PlannerExecutionException(
                        "C++ planner timed out after " + timeout.toMillis() + " ms");
            }

            String output = stdout.join();
            String error = stderr.join();
            if (process.exitValue() != 0) {
                throw new PlannerExecutionException(
                        "C++ planner exited with code " + process.exitValue() + ": " + error.trim());
            }

            try {
                return objectMapper.readValue(output, PlanResult.class);
            } catch (JsonProcessingException exception) {
                throw new PlannerExecutionException("C++ planner returned malformed JSON", exception);
            }
        } catch (IOException exception) {
            throw new PlannerExecutionException(
                    "Could not start or communicate with the C++ planner", exception);
        } catch (InterruptedException exception) {
            Thread.currentThread().interrupt();
            throw new PlannerExecutionException("Interrupted while waiting for the C++ planner", exception);
        } finally {
            if (process != null && process.isAlive()) {
                process.destroyForcibly();
            }
        }
    }

    private String readStream(java.io.InputStream stream) {
        try (stream) {
            return new String(stream.readAllBytes(), StandardCharsets.UTF_8);
        } catch (IOException exception) {
            throw new PlannerExecutionException("Could not read C++ planner output", exception);
        }
    }
}
