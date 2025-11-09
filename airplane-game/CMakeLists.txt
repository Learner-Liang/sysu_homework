cmake_minimum_required(VERSION 3.16)

project(airplane-game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Auto-handle Qt's meta-object compilation (MOC), user interface compilation (UIC), and resource compilation (RCC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt6 components
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Add executable with source files
add_executable(airplane-game
    main.cpp
    gamewindow.cpp
    gamewindow.h
    game.cpp
    game.h
    player.cpp
    player.h
    enemy.cpp
    enemy.h
    bullet.cpp
    bullet.h
)

# Link Qt6::Widgets library
target_link_libraries(airplane-game Qt6::Widgets)
