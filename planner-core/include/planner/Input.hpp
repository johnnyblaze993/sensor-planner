#pragma once

#include "planner/Models.hpp"

#include <filesystem>
#include <string>

namespace sensor_planner {

PlanRequest parseJson(const std::string& text);
PlanRequest parseCsv(const std::string& text);
PlanRequest parseXml(const std::string& text);
PlanRequest readRequestFile(const std::filesystem::path& path);
std::string toJson(const PlanResult& result);

}  // namespace sensor_planner
