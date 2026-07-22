#pragma once

#include "planner/Models.hpp"

namespace sensor_planner {

class Planner {
public:
    // const Coordinate& means "borrow this object without copying or changing
    // it." This is one of the most common parameter forms in modern C++.
    [[nodiscard]] double calculateDistanceKm(
        const Coordinate& start,
        const Coordinate& end) const;

    [[nodiscard]] bool hasRadioLineOfSight(
        const Coordinate& sensor,
        const Coordinate& target) const;

    [[nodiscard]] double calculateScore(
        const Coordinate& sensor,
        const Coordinate& target,
        double distanceKm,
        bool visible) const;

    [[nodiscard]] PlanResult createPlan(const PlanRequest& request) const;

    static void validateCoordinate(const Coordinate& coordinate);
};

}  // namespace sensor_planner
