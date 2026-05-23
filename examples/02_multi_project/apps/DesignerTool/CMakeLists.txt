qt_add_executable(DesignerTool
    WIN32 MACOSX_BUNDLE
    main.cpp
)

target_link_libraries(DesignerTool
    PRIVATE
        Qt::Widgets
        Study::UiKit
)

add_subdirectory(plugins/ColorPanel)

install(TARGETS DesignerTool
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
