
project( navigationarrow )

find_package( Qt6Core REQUIRED )
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

add_executable( navigationarrow ${SRC} )

target_link_libraries( navigationarrow QtMWidgets Qt6::Widgets Qt6::Gui Qt6::Core )
