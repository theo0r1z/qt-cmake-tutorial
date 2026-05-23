qt_add_library(CoreKit STATIC
    include/CoreKit/projectinfo.h
    src/projectinfo.cpp
)

target_include_directories(CoreKit
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(CoreKit
    PUBLIC
        Qt::Core
)

add_library(Study::CoreKit ALIAS CoreKit)
