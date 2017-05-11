
TEMPLATE = app
TARGET = example.xml.app
DESTDIR = .
CONFIG += console c++14
QT -= gui
QT += xml
DEFINES += CFGFILE_QT_SUPPORT CFGFILE_XML_SUPPORT

HEADERS += cfg.hpp

SOURCES += cfg.cpp \
           main.cpp

TEST_CFG_FILES += example.cfg

include( ../example.pri )

INCLUDEPATH += $$PWD/../..
DEPENDPATH += $$PWD/../..
