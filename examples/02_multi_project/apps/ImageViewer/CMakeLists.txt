qt_add_executable(ImageViewer
    WIN32 MACOSX_BUNDLE
    main.cpp
)

target_link_libraries(ImageViewer
    PRIVATE
        Qt::Widgets
        Study::CoreKit
)

install(TARGETS ImageViewer
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
