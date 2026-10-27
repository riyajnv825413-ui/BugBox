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

    # --- Bug generator engine (teammate module) ---
    bugenginebridge.h
    bugenginebridge.cpp
    bugengineadapter.h
    bugengineadapter.cpp

    include/bug_engine/Bug.h
    include/bug_engine/BugData.h
    include/bug_engine/BugGenerator.h
    include/bug_engine/ArrayBug.h
    include/bug_engine/ConditionBug.h
    include/bug_engine/LogicBug.h
    include/bug_engine/LoopBug.h
    include/bug_engine/NestedLoopBug.h
    include/bug_engine/RecursionBug.h
    include/bug_engine/SyntaxBug.h

    src/bug_engine/Bug.cpp
    src/bug_engine/BugGenerator.cpp
    src/bug_engine/ArrayBug.cpp
    src/bug_engine/ConditionBug.cpp
    src/bug_engine/LogicBug.cpp
    src/bug_engine/LoopBug.cpp
    src/bug_engine/NestedLoopBug.cpp
    src/bug_engine/RecursionBug.cpp
    src/bug_engine/SyntaxBug.cpp

    # --- Leaderboard (teammate module) ---
    leaderboard.h
    leaderboard.cpp
    leaderboarddialog.h
    leaderboarddialog.cpp
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

# The bug engine headers include each other by bare filename
# (e.g. #include "BugGenerator.h"), so expose both roots.
target_include_directories(bugboxwidget
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/bug_engine
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
