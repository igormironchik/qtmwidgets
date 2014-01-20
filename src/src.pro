
TEMPLATE = lib
TARGET = QtMWidgets
DESTDIR = ../lib
DEPENDPATH += . private
INCLUDEPATH += . ..
CONFIG += qt staticlib
QT += core gui widgets

HEADERS += private/drawing.hpp \
           private/datetimeparser.hpp \
           private/color.hpp \
           picker.hpp \
           timepicker.hpp \
           datepicker.hpp \
           datetimepicker.hpp \
           switch.hpp

SOURCES += private/drawing.cpp \
           private/datetimeparser.cpp \
           private/color.cpp \
           picker.cpp \
           timepicker.cpp \
           datepicker.cpp \
           datetimepicker.cpp \
           switch.cpp
