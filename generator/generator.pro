
TEMPLATE = app
TARGET = cfgfile.generator
DESTDIR = ..
QT -= core gui
CONFIG += console c++14

HEADERS += generator.hpp \
	cfg.hpp

SOURCES += generator.cpp \
	main.cpp \
	cfg.cpp

INCLUDEPATH += $$PWD/.. $$PWD/../Args
DEPENDPATH += $$PWD/.. $$PWD/../Args

include( ../cfgfile/cfgfile.pri )
