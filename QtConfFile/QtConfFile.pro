
TEMPLATE = lib
TARGET = QtConfFile
DESTDIR = ../lib
DEPENDPATH += . private
INCLUDEPATH += . ..
CONFIG += qt staticlib

HEADERS += private/constraint.hpp \
           private/input_stream.hpp \
           private/lex.hpp \
           private/parser.hpp \
           private/parser_info.hpp \
           private/tag.hpp \
           constraint_min_max.hpp \
           constraint_one_of.hpp \
           exceptions.hpp \
           tag_no_value.hpp \
           tag_scalar.hpp \
           tag_scalar_vector.hpp \
           tag_vector_of_tags.hpp

SOURCES += private/input_stream.cpp \
           private/lex.cpp \
           private/parser.cpp \
           private/parser_info.cpp \
           private/tag.cpp \
           exceptions.cpp \
           tag_no_value.cpp
