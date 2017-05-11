
TEMPLATE = app
DESTDIR = .
CONFIG += console c++14
QT -= gui
DEFINES += CFGFILE_QT_SUPPORT

SOURCES += main.cpp

INCLUDEPATH += $$PWD/../../..
DEPENDPATH += $$PWD/../../..
