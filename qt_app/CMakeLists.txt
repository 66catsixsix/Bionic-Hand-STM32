cmake_minimum_required(VERSION 3.16)
project(BionicHandHost VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Gui SerialPort)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(PROJECT_SOURCES
    src/main.cpp
    src/HandController.cpp
    src/HandController.h
    qml.qrc
)

add_executable(BionicHandHost
    ${PROJECT_SOURCES}
)

target_link_libraries(BionicHandHost
  PRIVATE Qt${QT_VERSION_MAJOR}::Core
  PRIVATE Qt${QT_VERSION_MAJOR}::Quick
  PRIVATE Qt${QT_VERSION_MAJOR}::Gui
  PRIVATE Qt${QT_VERSION_MAJOR}::SerialPort
)

set_target_properties(BionicHandHost PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
