cmake_minimum_required(VERSION 3.24)
project(sensor_planner_core VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
    URL_HASH SHA256=42f6e95cad6ec532fd372391373363b62a14af6d771056dbfc86160e6dfff7aa
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

add_library(planner_lib
    src/Planner.cpp
    src/Input.cpp
)
target_include_directories(planner_lib PUBLIC include)
target_link_libraries(planner_lib PUBLIC nlohmann_json::nlohmann_json)
target_compile_options(planner_lib PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
)

add_executable(planner-core src/main.cpp)
target_link_libraries(planner-core PRIVATE planner_lib)

include(CTest)
if(BUILD_TESTING)
    add_executable(planner-tests tests/PlannerTests.cpp)
    target_link_libraries(planner-tests PRIVATE planner_lib)
    add_test(NAME planner-tests COMMAND planner-tests)
endif()
