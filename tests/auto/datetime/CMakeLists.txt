
project( test.datetime )

find_package( Qt5Core 5.15 REQUIRED )
find_package( Qt5Test 5.15 REQUIRED )
find_package( Qt5Gui 5.15 REQUIRED )
find_package( Qt5Widgets 5.15 REQUIRED )

set( CMAKE_AUTOMOC ON )

if( ENABLE_COVERAGE )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage" )
endif( ENABLE_COVERAGE )

set( SRC main.cpp )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../../include
	${CMAKE_CURRENT_BINARY_DIR} )

link_directories( ${CMAKE_CURRENT_BINARY_DIR}/../../../lib )

add_executable( test.datetime ${SRC} )

target_link_libraries( test.datetime QtMWidgets Qt5::Widgets Qt5::Gui Qt5::Test Qt5::Core )

add_test( NAME test.datetime
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.datetime
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
