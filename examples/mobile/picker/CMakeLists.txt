
cmake_minimum_required( VERSION 2.8.11 )

project( picker )

find_package( Qt6Core REQUIRED )
find_package( Qt6Gui REQUIRED )
find_package( Qt6Widgets REQUIRED )

set( CMAKE_AUTOMOC ON )

set( SRC main.cpp )

set( CMAKE_ANDROID_ASSETS_DIRECTORIES android )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../../../include )

add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../../../src ${CMAKE_CURRENT_BINARY_DIR}/QtMWidgets )

add_executable( picker ${SRC} )

add_dependencies( picker QtMWidgets )

target_link_libraries( picker QtMWidgets Qt6::Widgets Qt6::Gui Qt6::Core )
