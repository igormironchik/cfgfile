
TEMPLATE = lib
TARGET = QtConfFile
DESTDIR = ../lib
DEPENDPATH += . private
INCLUDEPATH += . ..
CONFIG += qt staticlib

HEADERS += private/input_stream.hpp \
           private/lex.hpp \
           private/parser.hpp \
           private/tag.hpp \
           exceptions.hpp

SOURCES += private/input_stream.cpp \
           private/lex.cpp \
           private/parser.cpp \
           private/tag.cpp \
           exceptions.cpp
