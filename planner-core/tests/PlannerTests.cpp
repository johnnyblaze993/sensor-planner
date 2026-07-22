#include "planner/Input.hpp"
#include "planner/Planner.hpp"

#include <cmath>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

int failures = 0;

void check(bool condition, const std::string& message) {
    if (!condition) {
        ++failures;
        std::cerr << "FAIL: " << message << '\n';
    }
}

void expectThrows(const std::function<void()>& operation, const std::string& message) {
    try {
        operation();
        check(false, message);
    } catch (const std::exception&) {
        check(true, message);
    }
}

}  // namespace

int main() {
    const sensor_planner::Planner planner;
    const sensor_planner::Coordinate huntsville{34.725, -86.650, 180.0};

    const double samePoint = planner.calculateDistanceKm(huntsville, huntsville);
    check(std::abs(samePoint) < 0.0001, "same coordinate has zero distance");

    const sensor_planner::Coordinate nearby{34.800, -86.500, 220.0};
    const double nearbyDistance = planner.calculateDistanceKm(huntsville, nearby);
    check(nearbyDistance > 15.0 && nearbyDistance < 17.0,
          "Haversine distance is approximately 16 km");

    const sensor_planner::PlanRequest request{
        huntsville,
        {{"far", {35.5, -86.7, 100.0}}, {"near", nearby}},
    };
    const auto result = planner.createPlan(request);
    check(result.rankedTargets.size() == 2, "all targets appear in result");
    check(result.rankedTargets.front().id == "near", "higher score is ranked first");
    check(result.rankedTargets[0].score >= result.rankedTargets[1].score,
          "scores are descending");

    expectThrows(
        [&planner] { (void)planner.createPlan({{91.0, 0.0, 0.0}, {{"x", {0, 0, 0}}}}); },
        "invalid latitude is rejected");
    expectThrows(
        [&planner, &huntsville] {
            (void)planner.createPlan({huntsville, {{"same", huntsville}, {"same", huntsville}}});
        },
        "duplicate IDs are rejected");

    const auto parsed = sensor_planner::parseJson(R"({
        "sensor":{"latitude":34.725,"longitude":-86.65,"altitudeMeters":180},
        "targets":[{"id":"t1","latitude":34.8,"longitude":-86.5,"altitudeMeters":220}]
    })");
    check(parsed.targets.front().id == "t1", "JSON request is parsed");

    const auto csv = sensor_planner::parseCsv(
        "type,id,latitude,longitude,altitudeMeters\n"
        "sensor,radar,34.725,-86.650,180\n"
        "target,t1,34.800,-86.500,220\n");
    check(csv.targets.size() == 1, "CSV request is parsed");

    const auto xml = sensor_planner::parseXml(
        R"(<scenario><sensor latitude="34.725" longitude="-86.650" altitudeMeters="180"/>)"
        R"(<targets><target id="t1" latitude="34.800" longitude="-86.500" altitudeMeters="220"/>)"
        R"(</targets></scenario>)");
    check(xml.targets.size() == 1, "XML request is parsed");

    if (failures == 0) {
        std::cout << "All planner tests passed\n";
    }
    return failures == 0 ? 0 : 1;
}
