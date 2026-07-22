package com.example.sensorplanner.service;

public class PlannerExecutionException extends RuntimeException {
    public PlannerExecutionException(String message) { super(message); }
    public PlannerExecutionException(String message, Throwable cause) { super(message, cause); }
}
