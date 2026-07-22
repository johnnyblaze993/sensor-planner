#pragma once

#include <string>
#include <vector>

namespace sensor_planner {

// A struct is a good fit for a small data-only type. Members are public by
// default, unlike a class where they are private by default.
struct Coordinate {
    double latitude{};
    double longitude{};
    double altitudeMeters{};
};

struct Target {
    std::string id;
    Coordinate position;
};

struct PlanRequest {
    Coordinate sensor;
    std::vector<Target> targets;
};

struct RankedTarget {
    std::string id;
    double distanceKm{};
    double score{};
    bool visible{};
};

struct PlanResult {
    std::vector<RankedTarget> rankedTargets;
};

}  // namespace sensor_planner
