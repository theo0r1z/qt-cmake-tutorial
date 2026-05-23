cmake_minimum_required(VERSION 3.19)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(MultiProjectExample LANGUAGES CXX)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    qt_standard_project_setup(REQUIRES 6.5 SUPPORTS_UP_TO 6.11)
endif()

add_subdirectory(libs/CoreKit)
add_subdirectory(libs/UiKit)
add_subdirectory(apps/DesignerTool)
add_subdirectory(apps/ImageViewer)
