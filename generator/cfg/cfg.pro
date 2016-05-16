
TEMPLATE = lib
TARGET = QtConfFile.Generator.Cfg
DESTDIR = ../../lib
INCLUDEPATH += .
CONFIG += qt staticlib
QT -= gui
QT += xml

HEADERS += cfg.hpp

SOURCES += cfg.cpp

INCLUDEPATH += $$PWD/../..
DEPENDPATH += $$PWD/../..
