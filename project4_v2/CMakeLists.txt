cmake_minimum_required(VERSION 3.16)

project(Project4_v2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Find and include the Qt6 package
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Set the executable
add_executable(Project4_v2 main.cpp haffumantree.cpp)

# Link against the Qt6 libraries
target_link_libraries(Project4_v2 Qt6::Widgets)

# Ensure the headers are accessible
include_directories(${CMAKE_SOURCE_DIR})