
project( test.slider )

find_package( Qt6Core REQUIRED )
find_package( Qt6Test REQUIRED )
find_package( Qt6Gui REQUIRED )
find_package( Qt6Widgets REQUIRED )

set( CMAKE_AUTOMOC ON )

if( ENABLE_COVERAGE )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov --coverage" )
endif( ENABLE_COVERAGE )

set( SRC main.cpp )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../../include
	${CMAKE_CURRENT_BINARY_DIR} )

link_directories( ${CMAKE_CURRENT_BINARY_DIR}/../../../lib )

add_executable( test.slider ${SRC} )

target_link_libraries( test.slider QtMWidgets Qt6::Widgets Qt6::Gui Qt6::Test Qt6::Core )

add_test( NAME test.slider
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.slider
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
