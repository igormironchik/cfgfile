
TEMPLATE = app
DESTDIR = .
CONFIG += console c++14
QT -= gui
QT += core xml
DEFINES += CFGFILE_QT_SUPPORT CFGFILE_XML_SUPPORT

SOURCES += main.cpp

INCLUDEPATH += $$PWD/../../../3rdparty $$PWD/../../..
DEPENDPATH += $$PWD/../../../3rdparty $$PWD/../../..
