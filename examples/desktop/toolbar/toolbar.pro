
TEMPLATE = app
TARGET = toolbar
DESTDIR = .
CONFIG += console
QT += core gui widgets

SOURCES += main.cpp

RESOURCES = resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../lib/ -lQtMWidgets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../lib/ -lQtMWidgets
else:unix: LIBS += -L$$OUT_PWD/../../../lib/ -lQtMWidgets

INCLUDEPATH += $$PWD/../../../include
DEPENDPATH += $$PWD/../../../include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../lib/libQtMWidgets.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../lib/libQtMWidgets.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../lib/QtMWidgets.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../lib/QtMWidgets.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../../lib/libQtMWidgets.a
