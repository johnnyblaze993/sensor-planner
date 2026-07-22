package com.example.sensorplanner.config;

import org.springframework.boot.context.properties.ConfigurationProperties;
import java.time.Duration;

@ConfigurationProperties(prefix = "planner.cpp")
public record PlannerProperties(String executablePath, Duration timeout) {
}
