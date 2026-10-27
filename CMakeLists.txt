cmake_minimum_required(VERSION 3.15)
project(BugGenerator3000 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Required automatically for Qt UI layout components
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt Libraries
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Include paths for all source code headers - THIS KILLS THE ERROR
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

# Locate your add_executable line and ensure WIN32 is present:
add_executable(BugGenerator3000 ${PROJECT_SOURCES})

# Target-link dependencies
target_link_libraries(BugGenerator3000 PRIVATE Qt6::Widgets)
