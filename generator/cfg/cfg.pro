
TEMPLATE = lib
TARGET = QtConfFile.Generator.Cfg
DESTDIR = ../../lib
INCLUDEPATH += .
CONFIG += qt staticlib
QT -= gui

HEADERS += cfg.hpp

SOURCES += cfg.cpp

INCLUDEPATH += $$PWD/../../QtConfFile
DEPENDPATH += $$PWD/../../QtConfFile
