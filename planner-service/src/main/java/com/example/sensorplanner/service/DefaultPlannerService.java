package com.example.sensorplanner.service;

import com.example.sensorplanner.model.PlanRequest;
import com.example.sensorplanner.model.PlanResult;
import org.springframework.stereotype.Service;

@Service
public class DefaultPlannerService implements PlannerService {
    private final NativePlannerClient nativePlannerClient;

    // Spring sees this single constructor and injects the matching component.
    public DefaultPlannerService(NativePlannerClient nativePlannerClient) {
        this.nativePlannerClient = nativePlannerClient;
    }

    @Override
    public PlanResult generatePlan(PlanRequest request) {
        return nativePlannerClient.execute(request);
    }
}
