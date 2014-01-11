
TEMPLATE = lib
TARGET = QtMWidgets
DESTDIR = ../lib
DEPENDPATH += . private
INCLUDEPATH += . ..
CONFIG += qt staticlib
QT += core gui widgets

HEADERS += private/drawing.hpp \
           private/datetimeparser.hpp \
           picker.hpp \
           timepicker.hpp \
           datepicker.hpp \
           datetimepicker.hpp

SOURCES += private/drawing.cpp \
           private/datetimeparser.cpp \
           picker.cpp \
           timepicker.cpp \
           datepicker.cpp \
           datetimepicker.cpp
