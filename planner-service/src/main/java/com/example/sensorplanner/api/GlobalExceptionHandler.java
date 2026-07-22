package com.example.sensorplanner.api;

import com.example.sensorplanner.service.PlannerExecutionException;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.MethodArgumentNotValidException;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.RestControllerAdvice;
import java.time.Instant;
import java.util.LinkedHashMap;
import java.util.Map;

@RestControllerAdvice
public class GlobalExceptionHandler {
    @ExceptionHandler(MethodArgumentNotValidException.class)
    public ResponseEntity<Map<String, Object>> handleValidation(MethodArgumentNotValidException error) {
        Map<String, String> fields = new LinkedHashMap<>();
        error.getBindingResult().getFieldErrors().forEach(
                field -> fields.put(field.getField(), field.getDefaultMessage()));
        return errorResponse(HttpStatus.BAD_REQUEST, "Request validation failed", fields);
    }

    @ExceptionHandler(PlannerExecutionException.class)
    public ResponseEntity<Map<String, Object>> handlePlannerFailure(PlannerExecutionException error) {
        return errorResponse(HttpStatus.BAD_GATEWAY, error.getMessage(), Map.of());
    }

    private ResponseEntity<Map<String, Object>> errorResponse(
            HttpStatus status, String message, Map<String, String> fields) {
        Map<String, Object> body = new LinkedHashMap<>();
        body.put("timestamp", Instant.now().toString());
        body.put("status", status.value());
        body.put("error", status.getReasonPhrase());
        body.put("message", message);
        if (!fields.isEmpty()) body.put("fields", fields);
        return ResponseEntity.status(status).body(body);
    }
}
