
TEMPLATE = lib
TARGET = QtMWidgets
DESTDIR = ../lib
DEPENDPATH += . private
INCLUDEPATH += . ..
CONFIG += qt staticlib
QT += core gui widgets

HEADERS += private/drawing.hpp \
           picker.hpp \
           timepicker.hpp

SOURCES += private/drawing.cpp \
           picker.cpp \
           timepicker.cpp
