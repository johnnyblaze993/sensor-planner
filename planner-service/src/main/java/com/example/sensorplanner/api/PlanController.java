package com.example.sensorplanner.api;

import com.example.sensorplanner.model.PlanRequest;
import com.example.sensorplanner.model.PlanResult;
import com.example.sensorplanner.service.PlannerService;
import jakarta.validation.Valid;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import java.util.Map;

@RestController
@RequestMapping("/api")
public class PlanController {
    private final PlannerService plannerService;

    public PlanController(PlannerService plannerService) {
        this.plannerService = plannerService;
    }

    @PostMapping("/plans")
    public PlanResult createPlan(@Valid @RequestBody PlanRequest request) {
        return plannerService.generatePlan(request);
    }

    @GetMapping("/health")
    public ResponseEntity<Map<String, String>> health() {
        return ResponseEntity.ok(Map.of("status", "UP"));
    }
}
