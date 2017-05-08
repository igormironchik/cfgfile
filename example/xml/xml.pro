
TEMPLATE = app
TARGET = example.xml.app
DESTDIR = .
CONFIG += console c++14
QT -= gui
QT += xml
DEFINES += CFGFILE_QSTRING_BUILD CFGFILE_XML_BUILD

HEADERS += cfg.hpp

SOURCES += cfg.cpp \
           main.cpp

TEST_CFG_FILES += example.cfg

include( ../example.pri )

INCLUDEPATH += $$PWD/../..
DEPENDPATH += $$PWD/../..
