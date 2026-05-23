qt_add_library(ColorPanelPlugin STATIC
    colorpanel.cpp
    colorpanel.h
)

target_link_libraries(ColorPanelPlugin
    PUBLIC
        Qt::Widgets
)

add_library(Study::ColorPanelPlugin ALIAS ColorPanelPlugin)
