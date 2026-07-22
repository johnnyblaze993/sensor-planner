#include "planner/Input.hpp"

#include <nlohmann/json.hpp>

#include <fstream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace sensor_planner {
namespace {

using json = nlohmann::json;

Coordinate coordinateFromJson(const json& value) {
    return {
        value.at("latitude").get<double>(),
        value.at("longitude").get<double>(),
        value.at("altitudeMeters").get<double>(),
    };
}

std::vector<std::string> splitCsvRow(const std::string& row) {
    // This intentionally small reader handles the project's synthetic CSV.
    // Production CSV should use a library to correctly support quoted commas.
    std::vector<std::string> fields;
    std::stringstream stream(row);
    std::string field;
    while (std::getline(stream, field, ',')) {
        fields.push_back(field);
    }
    return fields;
}

Coordinate coordinateFromAttributes(const std::string& attributes) {
    const auto read = [&attributes](const std::string& name) {
        const std::regex pattern(name + "\\s*=\\s*\"([^\"]+)\"");
        std::smatch match;
        if (!std::regex_search(attributes, match, pattern)) {
            throw std::invalid_argument("missing XML attribute: " + name);
        }
        return match[1].str();
    };
    return {std::stod(read("latitude")), std::stod(read("longitude")),
            std::stod(read("altitudeMeters"))};
}

}  // namespace

PlanRequest parseJson(const std::string& text) {
    try {
        const json document = json::parse(text);
        PlanRequest request{coordinateFromJson(document.at("sensor")), {}};
        for (const json& value : document.at("targets")) {
            request.targets.push_back({
                value.at("id").get<std::string>(),
                coordinateFromJson(value),
            });
        }
        return request;
    } catch (const json::exception& error) {
        throw std::invalid_argument(std::string("invalid JSON request: ") + error.what());
    }
}

PlanRequest parseCsv(const std::string& text) {
    std::stringstream input(text);
    std::string row;
    std::getline(input, row);  // Skip: type,id,latitude,longitude,altitudeMeters

    PlanRequest request{};
    bool foundSensor = false;
    std::size_t lineNumber = 1;
    while (std::getline(input, row)) {
        ++lineNumber;
        if (row.empty()) {
            continue;
        }
        const auto fields = splitCsvRow(row);
        if (fields.size() != 5) {
            throw std::invalid_argument("CSV line " + std::to_string(lineNumber) +
                                        " must contain 5 fields");
        }
        const Coordinate coordinate{
            std::stod(fields[2]), std::stod(fields[3]), std::stod(fields[4])};
        if (fields[0] == "sensor") {
            if (foundSensor) {
                throw std::invalid_argument("CSV must contain exactly one sensor row");
            }
            request.sensor = coordinate;
            foundSensor = true;
        } else if (fields[0] == "target") {
            request.targets.push_back({fields[1], coordinate});
        } else {
            throw std::invalid_argument("CSV row type must be sensor or target");
        }
    }
    if (!foundSensor) {
        throw std::invalid_argument("CSV is missing a sensor row");
    }
    return request;
}

PlanRequest parseXml(const std::string& text) {
    // The restricted XML shape is intentional for this two-week exercise.
    // Never use regular expressions for arbitrary/untrusted production XML.
    const std::regex sensorPattern(R"(<sensor\s+([^>]*)/>)");
    const std::regex targetPattern(R"(<target\s+([^>]*)/>)");
    std::smatch sensorMatch;
    if (!std::regex_search(text, sensorMatch, sensorPattern)) {
        throw std::invalid_argument("XML is missing a self-closing sensor element");
    }

    PlanRequest request{coordinateFromAttributes(sensorMatch[1].str()), {}};
    for (auto iterator = std::sregex_iterator(text.begin(), text.end(), targetPattern);
         iterator != std::sregex_iterator(); ++iterator) {
        const std::string attributes = (*iterator)[1].str();
        const std::regex idPattern("id\\s*=\\s*\"([^\"]+)\"");
        std::smatch idMatch;
        if (!std::regex_search(attributes, idMatch, idPattern)) {
            throw std::invalid_argument("target is missing XML attribute: id");
        }
        request.targets.push_back({idMatch[1].str(), coordinateFromAttributes(attributes)});
    }
    return request;
}

PlanRequest readRequestFile(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("could not open input file: " + path.string());
    }
    const std::string text((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
    const std::string extension = path.extension().string();
    if (extension == ".json") return parseJson(text);
    if (extension == ".csv") return parseCsv(text);
    if (extension == ".xml") return parseXml(text);
    throw std::invalid_argument("input extension must be .json, .csv, or .xml");
}

std::string toJson(const PlanResult& result) {
    json output;
    output["rankedTargets"] = json::array();
    for (const RankedTarget& target : result.rankedTargets) {
        output["rankedTargets"].push_back({
            {"id", target.id},
            {"distanceKm", target.distanceKm},
            {"score", target.score},
            {"visible", target.visible},
        });
    }
    return output.dump(2);
}

}  // namespace sensor_planner
