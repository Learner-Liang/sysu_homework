cmake_minimum_required(VERSION 3.16)

project(Project4_v2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
    main.cpp
    mainwindow.cpp
    haffumantree.cpp
)

set(HEADERS
    mainwindow.h
    haffumantree.h
)

set(UI
    mainwindow.ui
)

qt_wrap_ui(UI_HEADERS ${UI})

add_executable(Project4_v2
    ${SOURCES}
    ${HEADERS}
    ${UI}
)

target_link_libraries(Project4_v2 PRIVATE Qt6::Widgets)
