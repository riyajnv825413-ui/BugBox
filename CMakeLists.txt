cmake_minimum_required(VERSION 3.15)
project(BugGenerator3000 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Handle UI compilation automatically for Rishika's screens
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Target the active framework installation
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Include paths for all source code headers
include_directories(
    src
    src/bug_engine
    src/compilation
    src/UI
)

# Recursively index all added team files
file(GLOB_RECURSE PROJECT_SOURCES
    "src/main.cpp"
    "src/bug_engine/*.cpp"
    "src/bug_engine/*.h"
    "src/bug_engine/*.hpp"
    "src/compilation/*.cpp"
    "src/compilation/*.h"
    "src/compilation/*.hpp"
    "src/UI/*.cpp"
    "src/UI/*.h"
    "src/UI/*.ui"
)

# Generate final cohesive executable binary
add_executable(BugGenerator3000 ${PROJECT_SOURCES})

# Target-link dependencies
target_link_libraries(BugGenerator3000 PRIVATE Qt6::Widgets)
