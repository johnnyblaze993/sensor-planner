package com.example.sensorplanner.service;

import com.example.sensorplanner.model.PlanRequest;
import com.example.sensorplanner.model.PlanResult;

// This interface is a seam: unit tests can replace the operating-system
// process with a small fake implementation.
public interface NativePlannerClient {
    PlanResult execute(PlanRequest request);
}
