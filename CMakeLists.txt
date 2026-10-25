cmake_minimum_required(VERSION 3.19)
project(bugboxwidget LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(bugboxwidget
    WIN32 MACOSX_BUNDLE

    # --- Entry point ---
    main.cpp

    # --- Main window (navigation controller) ---
    mainwindow.h
    mainwindow.cpp

    # --- Screen widgets ---
    homescreen.h
    homescreen.cpp

    debugscreen.h
    debugscreen.cpp

    resultscreen.h
    resultscreen.cpp

    # --- Data model ---
    bugchallenge.h
    bugchallenge.cpp

    # --- Mock adapter (replace with real module when available) ---
    bugchallengeadapter.h
    bugchallengeadapter.cpp

    # --- Score manager ---
    scoremanager.h
    scoremanager.cpp

    # --- Compilation engine (teammate module) ---
    CompilerEngine.h
    CompilerEngine.cpp
)

qt_add_translations(
    TARGETS bugboxwidget
    TS_FILES bugboxwidget_en_IN.ts
)

target_link_libraries(bugboxwidget
    PRIVATE
        Qt6::Core
        Qt6::Widgets
)

install(TARGETS bugboxwidget
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET bugboxwidget
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
