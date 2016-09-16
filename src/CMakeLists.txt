
project( QtMWidgets )


set( SRC picker.hpp
	pageview.hpp
	progressbar.cpp
	lineedit.hpp
	stepper.cpp
	abstractlistmodel.hpp
	abstractscrollarea.cpp
	toolbar.hpp
	navigationbutton.cpp
	busyindicator.hpp
	fingergeometry.hpp
	datetimepicker.cpp
	lineedit.cpp
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
	textedit.cpp
	color.cpp
	datetimepicker.hpp
	stepper.hpp
	datepicker.hpp
	scroller.hpp
	pagecontrol.cpp
	textedit.hpp
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
	private/cursorshifter.hpp
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
	private/cursorshifter.cpp
	slider.cpp
	busyindicator.cpp
	tableview.hpp
	textlabel.hpp
	navigationarrow.hpp
	listmodel.hpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../include
	${CMAKE_CURRENT_SOURCE_DIR} )

include_directories( ${Qt5Core_INCLUDE_DIRS}
	${Qt5Gui_INCLUDE_DIRS}
	${Qt5Widgets_INCLUDE_DIRS} )

add_library( QtMWidgets STATIC ${SRC} )

set_property( TARGET QtMWidgets PROPERTY CXX_STANDARD 14 )

