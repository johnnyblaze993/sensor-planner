package com.example.sensorplanner.model;

import jakarta.validation.Valid;
import jakarta.validation.constraints.NotEmpty;
import jakarta.validation.constraints.NotNull;
import java.util.List;

public record PlanRequest(
        @NotNull @Valid Coordinate sensor,
        @NotEmpty List<@Valid Target> targets) {
}
