package com.example.sensorplanner.model;

public record RankedTarget(String id, double distanceKm, double score, boolean visible) {
}
