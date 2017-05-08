
TEMPLATE = app
TARGET = example.simple.app
DESTDIR = .
CONFIG += console c++14
QT -= gui core

HEADERS += cfg.hpp

SOURCES += cfg.cpp \
           main.cpp

TEST_CFG_FILES += example.cfg

include( ../example.pri )

INCLUDEPATH += $$PWD/../..
DEPENDPATH += $$PWD/../..
