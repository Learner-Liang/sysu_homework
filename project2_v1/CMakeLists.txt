cmake_minimum_required(VERSION 3.16)

project(project2_v1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 自动处理 Qt 的元对象编译 (MOC), 用户界面编译 (UIC), 和资源编译 (RCC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 查找 Qt6 组件
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# 添加可执行文件及其源文件
add_executable(project2_v1
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    tire_tree.cpp
    tire_tree.h
)

# 链接 Qt6::Widgets 库
target_link_libraries(project2_v1 Qt6::Widgets)
