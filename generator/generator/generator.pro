
TEMPLATE = app
TARGET = qtconffile.generator
DESTDIR = ../..
QT += core
CONFIG += console

HEADERS += generator.hpp

SOURCES += generator.cpp \
           main.cpp

unix|win32: LIBS += -L$$OUT_PWD/../../lib/ -lQtConfFile.Generator.Cfg -lQtConfFile

INCLUDEPATH += $$PWD/.. $$PWD/../..
DEPENDPATH += $$PWD/.. $$PWD/../..

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../lib/QtConfFile.Generator.Cfg.lib \
									$$OUT_PWD/../../lib/QtConfFile.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../lib/libQtConfFile.Generator.Cfg.a \
									   $$OUT_PWD/../../lib/libQtConfFile.a
