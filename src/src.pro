
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
           color.hpp \
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
           fingergeometry.hpp \
           textlabel.hpp \
           slider.hpp \
           busyindicator.hpp \
           lineedit.hpp \
           private/cursorshifter.hpp \
           textedit.hpp \
           navigationbar.hpp \
           navigationbutton.hpp \
           navigationarrow.hpp \
           scroller.hpp \
           stepper.hpp \
           toolbar.hpp \
           pageview.hpp \
           pagecontrol.hpp \
           progressbar.hpp \
           listmodel.hpp \
           abstractlistview.hpp \
           private/listmodel_p.hpp \
           abstractlistmodel.hpp \
           messagebox.hpp \
           private/messageboxbutton.hpp

SOURCES += private/drawing.cpp \
           private/datetimeparser.cpp \
           color.cpp \
           picker.cpp \
           timepicker.cpp \
           datepicker.cpp \
           datetimepicker.cpp \
           switch.cpp \
           scrollarea.cpp \
           abstractscrollarea.cpp \
           private/layoutengine.cpp \
           tableview.cpp \
           fingergeometry.cpp \
           textlabel.cpp \
           slider.cpp \
           busyindicator.cpp \
           lineedit.cpp \
           private/cursorshifter.cpp \
           textedit.cpp \
           navigationbar.cpp \
           navigationbutton.cpp \
           navigationarrow.cpp \
           scroller.cpp \
           stepper.cpp \
           toolbar.cpp \
           pageview.cpp \
           pagecontrol.cpp \
           progressbar.cpp \
           abstractlistview.cpp \
           messagebox.cpp \
           private/messageboxbutton.cpp
