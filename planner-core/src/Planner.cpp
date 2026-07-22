#include "planner/Planner.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <unordered_set>

namespace sensor_planner {
namespace {

constexpr double kEarthRadiusKm = 6371.0;
constexpr double kDegreesToRadians = 3.14159265358979323846 / 180.0;

double toRadians(double degrees) {
    return degrees * kDegreesToRadians;
}

}  // namespace

void Planner::validateCoordinate(const Coordinate& coordinate) {
    if (!std::isfinite(coordinate.latitude) || coordinate.latitude < -90.0 ||
        coordinate.latitude > 90.0) {
        throw std::invalid_argument("latitude must be a finite number from -90 to 90");
    }
    if (!std::isfinite(coordinate.longitude) || coordinate.longitude < -180.0 ||
        coordinate.longitude > 180.0) {
        throw std::invalid_argument("longitude must be a finite number from -180 to 180");
    }
    if (!std::isfinite(coordinate.altitudeMeters) || coordinate.altitudeMeters < 0.0) {
        throw std::invalid_argument("altitudeMeters must be a finite non-negative number");
    }
}

double Planner::calculateDistanceKm(
    const Coordinate& start,
    const Coordinate& end) const {
    validateCoordinate(start);
    validateCoordinate(end);

    // The Haversine formula estimates great-circle distance on a sphere.
    // Altitude is intentionally excluded here; this is surface distance.
    const double latitudeDelta = toRadians(end.latitude - start.latitude);
    const double longitudeDelta = toRadians(end.longitude - start.longitude);
    const double startLatitude = toRadians(start.latitude);
    const double endLatitude = toRadians(end.latitude);

    const double a = std::pow(std::sin(latitudeDelta / 2.0), 2.0) +
                     std::cos(startLatitude) * std::cos(endLatitude) *
                         std::pow(std::sin(longitudeDelta / 2.0), 2.0);
    const double centralAngle = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));
    return kEarthRadiusKm * centralAngle;
}

bool Planner::hasRadioLineOfSight(
    const Coordinate& sensor,
    const Coordinate& target) const {
    const double distanceKm = calculateDistanceKm(sensor, target);

    // Approximate radio horizon in km. This uses the common effective-Earth
    // approximation and assumes clear terrain. It is a teaching model, not an
    // operational radar calculation.
    const double horizonKm = 4.12 *
        (std::sqrt(sensor.altitudeMeters) + std::sqrt(target.altitudeMeters));
    return distanceKm <= horizonKm;
}

double Planner::calculateScore(
    const Coordinate& sensor,
    const Coordinate& target,
    double distanceKm,
    bool visible) const {
    if (!std::isfinite(distanceKm) || distanceKm < 0.0) {
        throw std::invalid_argument("distanceKm must be a finite non-negative number");
    }

    // All three parts are normalized to 0..1, which makes the weights easy to
    // explain and change. This is deliberately a learning-oriented heuristic.
    const double distanceScore = 1.0 / (1.0 + distanceKm / 50.0);
    const double combinedAltitude = sensor.altitudeMeters + target.altitudeMeters;
    const double altitudeScore = std::clamp(combinedAltitude / 2000.0, 0.0, 1.0);
    const double visibilityScore = visible ? 1.0 : 0.0;

    return std::clamp(
        0.60 * distanceScore + 0.20 * altitudeScore + 0.20 * visibilityScore,
        0.0,
        1.0);
}

PlanResult Planner::createPlan(const PlanRequest& request) const {
    validateCoordinate(request.sensor);
    if (request.targets.empty()) {
        throw std::invalid_argument("at least one target is required");
    }

    PlanResult result;
    result.rankedTargets.reserve(request.targets.size());
    std::unordered_set<std::string> ids;

    for (const Target& target : request.targets) {
        if (target.id.empty()) {
            throw std::invalid_argument("target id must not be blank");
        }
        if (!ids.insert(target.id).second) {
            throw std::invalid_argument("target ids must be unique: " + target.id);
        }

        validateCoordinate(target.position);
        const double distanceKm = calculateDistanceKm(request.sensor, target.position);
        const bool visible = hasRadioLineOfSight(request.sensor, target.position);
        result.rankedTargets.push_back({
            target.id,
            distanceKm,
            calculateScore(request.sensor, target.position, distanceKm, visible),
            visible,
        });
    }

    std::sort(
        result.rankedTargets.begin(),
        result.rankedTargets.end(),
        [](const RankedTarget& left, const RankedTarget& right) {
            if (left.score == right.score) {
                return left.id < right.id;  // Stable, deterministic tie-breaker.
            }
            return left.score > right.score;
        });
    return result;
}

}  // namespace sensor_planner
