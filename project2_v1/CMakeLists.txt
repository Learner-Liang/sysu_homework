# CMakeLists.txt for Qt project
cmake_minimum_required(VERSION 3.5)
project(project2_v1)
set(CMAKE_CXX_STANDARD 11)
find_package(Qt5Widgets REQUIRED)

add_executable(project2_v1 main.cpp mainwindow.cpp tire_tree.cpp)
target_link_libraries(project2_v1 Qt5::Widgets)
