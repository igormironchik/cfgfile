
TEMPLATE = app
TARGET = cfgfile.generator
DESTDIR = ..
QT -= core gui
CONFIG += console

HEADERS += generator.hpp \
	cfg.hpp

SOURCES += generator.cpp \
	main.cpp \
	cfg.cpp

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

include( ../cfgfile/cfgfile.pri )
