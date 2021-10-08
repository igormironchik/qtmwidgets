
project( QtMWidgets )

if( ENABLE_COVERAGE )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage" )
endif( ENABLE_COVERAGE )

find_package( Qt6Core REQUIRED )
find_package( Qt6Gui REQUIRED )
find_package( Qt6Widgets REQUIRED )

set( SRC picker.hpp
	pageview.hpp
	progressbar.cpp
	stepper.cpp
	abstractlistmodel.hpp
	abstractscrollarea.cpp
	toolbar.hpp
	navigationbutton.cpp
	busyindicator.hpp
	fingergeometry.hpp
	datetimepicker.cpp
	abstractlistview.cpp
	abstractscrollarea.hpp
	scroller.cpp
	color.hpp
	fingergeometry.cpp
	navigationarrow.cpp
	abstractlistview.hpp
	messagebox.hpp
	pagecontrol.hpp
	textlabel.cpp
	timepicker.hpp
	tableview.cpp
	slider.hpp
	messagebox.cpp
	color.cpp
	datetimepicker.hpp
	stepper.hpp
	datepicker.hpp
	scroller.hpp
	pagecontrol.cpp
	timepicker.cpp
	scrollarea.hpp
	navigationbar.cpp
	datepicker.cpp
	pageview.cpp
	toolbar.cpp
	switch.cpp
	navigationbar.hpp
	picker.cpp
	progressbar.hpp
	scrollarea.cpp
	switch.hpp
	navigationbutton.hpp
	private/layoutengine.cpp
	private/datetimeparser.hpp
	private/datetimeparser.cpp
	private/drawing.cpp
	private/drawing.hpp
	private/messageboxbutton.cpp
	private/abstractscrollarea_p.hpp
	private/tableview_p.hpp
	private/messageboxbutton.hpp
	private/scrollarea_p.hpp
	private/listmodel_p.hpp
	private/layoutengine.hpp
	slider.cpp
	busyindicator.cpp
	tableview.hpp
	textlabel.hpp
	navigationarrow.hpp
	listmodel.hpp
	private/utils.hpp
	private/utils.cpp )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../include
	${CMAKE_CURRENT_SOURCE_DIR} )

add_library( QtMWidgets STATIC ${SRC} )

target_link_libraries( QtMWidgets Qt6::Widgets Qt6::Gui Qt6::Core )

set_property( TARGET QtMWidgets PROPERTY CXX_STANDARD 14 )

