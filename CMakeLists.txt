cmake_minimum_required(VERSION 3.19)

project(StudyCMake
    VERSION 1.0.0
    DESCRIPTION "Qt CMake tutorial examples"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(STUDY_BUILD_WIDGETS_BASIC "Build the basic Qt Widgets example" OFF)
option(STUDY_BUILD_MULTI_PROJECT "Build the multi-project Qt example" OFF)
option(STUDY_BUILD_QML_APP "Build the Qt Quick/QML example" OFF)

if(STUDY_BUILD_WIDGETS_BASIC OR STUDY_BUILD_MULTI_PROJECT OR STUDY_BUILD_QML_APP)
    find_package(Qt6 6.5 REQUIRED COMPONENTS Core)
    qt_standard_project_setup(REQUIRES 6.5 SUPPORTS_UP_TO 6.11)
endif()

if(STUDY_BUILD_WIDGETS_BASIC)
    add_subdirectory(examples/01_widgets_basic)
endif()

if(STUDY_BUILD_MULTI_PROJECT)
    add_subdirectory(examples/02_multi_project)
endif()

if(STUDY_BUILD_QML_APP)
    add_subdirectory(examples/03_qml_app)
endif()
