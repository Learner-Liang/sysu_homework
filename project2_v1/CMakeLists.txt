cmake_minimum_required(VERSION 3.5)
project(project2_v1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
add_executable(project2_v1
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  tire_tree.cpp
  tire_tree.h
)
target_link_libraries(project2_v1 PRIVATE Qt6::Widgets)
