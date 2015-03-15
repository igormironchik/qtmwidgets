
TEMPLATE = subdirs

SUBDIRS = src \
          examples

examples.depends = src

libDir.commands = mkdir lib

QMAKE_EXTRA_TARGETS += libDir
