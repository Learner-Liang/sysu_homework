cmake_minimum_required(VERSION 3.5)

project(HuffmanCompressionGUI)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_PREFIX_PATH "path/to/Qt/lib/cmake") # Adjust this to your Qt installation path

find_package(Qt5Widgets REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

# Adding sources
set(SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    ui_mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../project4/haffumantree.cpp
)

# Adding UI files
set(UI_FILES
    mainwindow.ui
)
qt5_wrap_ui(UI_HEADERS ${UI_FILES})

add_executable(${PROJECT_NAME} ${SOURCES} ${UI_HEADERS})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)