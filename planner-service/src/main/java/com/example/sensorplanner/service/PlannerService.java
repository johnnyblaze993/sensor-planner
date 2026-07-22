package com.example.sensorplanner.service;

import com.example.sensorplanner.model.PlanRequest;
import com.example.sensorplanner.model.PlanResult;

public interface PlannerService {
    PlanResult generatePlan(PlanRequest request);
}
