
cmake_minimum_required( VERSION 2.8.11 )

project( QtMWidgets )

# Find includes in corresponding build directories
set( CMAKE_INCLUDE_CURRENT_DIR ON )
# Instruct CMake to run moc automatically when needed.
set( CMAKE_AUTOMOC ON )

if( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" )
  set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fPIC" )
endif()

find_package( Qt5Core 5.15 REQUIRED )
find_package( Qt5Gui 5.15 REQUIRED )
find_package( Qt5Widgets 5.15 REQUIRED )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )

add_subdirectory( src )
