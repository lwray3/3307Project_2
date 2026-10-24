cmake_minimum_required(VERSION 3.16)

project(CasinoGame_3307 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(CasinoGame_3307
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    blackjackwindow.cpp
    blackjackwindow.h
    blackjackwindow.ui
    pokerwindow.h
    pokerwindow.cpp
    pokerwindow.ui 

    backend/Blackjack.cpp
    backend/Blackjack.h
    backend/Card.cpp
    backend/Card.h
    backend/Deck.cpp
    backend/Deck.h
    backend/Player.cpp
    backend/Player.h
    backend/IGame.h
    backend/GameManager.h
    backend/Poker.cpp
    backend/Poker.h
    backend/CardWidget.h
    backend/CardWidget.cpp
    
)

target_link_libraries(CasinoGame_3307 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(CasinoGame_3307 PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CasinoGame_3307
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)