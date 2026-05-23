cmake_minimum_required(VERSION 3.19)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(QmlAppExample LANGUAGES CXX)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Qml Quick)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    qt_standard_project_setup(REQUIRES 6.5 SUPPORTS_UP_TO 6.11)
endif()

qt_add_executable(QmlDashboard
    main.cpp
)

qt_add_qml_module(QmlDashboard
    URI Study.Dashboard
    VERSION 1.0
    QML_FILES
        Main.qml
        components/MetricCard.qml
)

target_link_libraries(QmlDashboard
    PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick
)

install(TARGETS QmlDashboard
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_qml_app_script(
    TARGET QmlDashboard
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
