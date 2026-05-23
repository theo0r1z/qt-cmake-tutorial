qt_add_library(UiKit STATIC
    include/UiKit/bannerwidget.h
    src/bannerwidget.cpp
)

target_include_directories(UiKit
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(UiKit
    PUBLIC
        Qt::Widgets
        Study::CoreKit
)

add_library(Study::UiKit ALIAS UiKit)
