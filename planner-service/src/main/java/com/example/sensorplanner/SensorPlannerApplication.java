package com.example.sensorplanner;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.context.properties.ConfigurationPropertiesScan;

@SpringBootApplication
@ConfigurationPropertiesScan
public class SensorPlannerApplication {
    public static void main(String[] args) {
        SpringApplication.run(SensorPlannerApplication.class, args);
    }
}
