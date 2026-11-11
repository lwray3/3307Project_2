
cmake_minimum_required(VERSION 3.16)

project(CasinoGame_3307 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(CasinoGame_3307
    main.cpp
    core/Blackjack.cpp
    core/Blackjack.h

    core/Baccarat.cpp
    core/Baccarat.h

    core/Card.cpp
    core/Card.h

    core/CardFactory.cpp
    core/CardFactory.h

    core/Deck.cpp
    core/Deck.h

    core/Player.cpp
    core/Player.h

    core/IGame.h
    core/GameManager.h

    core/Poker.cpp
    core/Poker.h

    core/pokerEvaluator.cpp
    core/pokerEvaluator.h

    core/GameFactory.cpp
    core/GameFactory.h

    core/CardWidget.cpp
    core/CardWidget.h

    ui/mainwindow.cpp
    ui/mainwindow.h
    ui/mainwindow.ui

    ui/blackjackwindow.cpp
    ui/blackjackwindow.h
    ui/blackjackwindow.ui

    ui/pokerwindow.cpp
    ui/pokerwindow.h
    ui/pokerwindow.ui

    baccaratwindow.cpp
    baccaratwindow.h
    baccaratwindow.ui

    ui/IGameAnimation.h
    ui/QtCardAnimation.cpp
    ui/QtCardAnimation.h

    ui/GameWindow.cpp
    ui/GameWindow.h


)

# Allow source files to include headers from both folders
target_include_directories(CasinoGame_3307 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

# Allow Qt AUTOUIC to locate .ui files
set_target_properties(CasinoGame_3307 PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui"
)

target_link_libraries(CasinoGame_3307 PRIVATE
    Qt6::Widgets
)

set_target_properties(CasinoGame_3307 PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING
        ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS CasinoGame_3307
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
