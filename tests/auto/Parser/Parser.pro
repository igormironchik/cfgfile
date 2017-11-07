
TEMPLATE = app
DESTDIR = .
CONFIG += console c++14
QT -= gui
DEFINES += CFGFILE_QT_SUPPORT
QT += core

SOURCES += main.cpp

INCLUDEPATH += $$PWD/../../..
DEPENDPATH += $$PWD/../../..
