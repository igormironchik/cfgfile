
TEMPLATE = app
DESTDIR = .
CONFIG += console c++14
QT -= gui
QT += core xml
DEFINES += CFGFILE_QT_SUPPORT CFGFILE_XML_SUPPORT

SOURCES += main.cpp

INCLUDEPATH += $$PWD/../../..
DEPENDPATH += $$PWD/../../..
