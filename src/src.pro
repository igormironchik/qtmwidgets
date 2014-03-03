
TEMPLATE = lib
TARGET = QtMWidgets
DESTDIR = ../lib
DEPENDPATH += . private
INCLUDEPATH += . ..
CONFIG += qt staticlib
QT += core gui widgets

OBJECTS_DIR = ./.obj
MOC_DIR = ./.moc

HEADERS += private/drawing.hpp \
           private/datetimeparser.hpp \
           private/color.hpp \
           picker.hpp \
           timepicker.hpp \
           datepicker.hpp \
           datetimepicker.hpp \
           switch.hpp \
           scrollarea.hpp \
           abstractscrollarea.hpp \
           private/abstractscrollarea_p.hpp \
           private/scrollarea_p.hpp \
           private/layoutengine.hpp \
           tableview.hpp \
           private/tableview_p.hpp \
           private/fingergeometry.hpp \
           textlabel.hpp \
           slider.hpp \
           busyindicator.hpp

SOURCES += private/drawing.cpp \
           private/datetimeparser.cpp \
           private/color.cpp \
           picker.cpp \
           timepicker.cpp \
           datepicker.cpp \
           datetimepicker.cpp \
           switch.cpp \
           scrollarea.cpp \
           abstractscrollarea.cpp \
           private/layoutengine.cpp \
           tableview.cpp \
           private/fingergeometry.cpp \
           textlabel.cpp \
           slider.cpp \
           busyindicator.cpp
