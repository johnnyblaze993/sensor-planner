#include "planner/Input.hpp"
#include "planner/Planner.hpp"

#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[]) {
    try {
        sensor_planner::PlanRequest request;
        if (argc == 1) {
            // Java writes one JSON document to stdin. Reading until EOF makes
            // the native program independent of line wrapping/pretty printing.
            const std::string input(
                std::istreambuf_iterator<char>(std::cin),
                std::istreambuf_iterator<char>());
            request = sensor_planner::parseJson(input);
        } else if (argc == 3 && std::string(argv[1]) == "--input") {
            request = sensor_planner::readRequestFile(argv[2]);
        } else {
            std::cerr << "Usage: planner-core [--input scenario.json|csv|xml]\n";
            return 2;
        }

        const sensor_planner::Planner planner;
        std::cout << sensor_planner::toJson(planner.createPlan(request)) << '\n';
        return 0;
    } catch (const std::exception& error) {
        // stdout is reserved for valid JSON, so callers can safely parse it.
        std::cerr << "planner error: " << error.what() << '\n';
        return 1;
    }
}
