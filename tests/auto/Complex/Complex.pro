
TEMPLATE = app
DESTDIR = .
CONFIG += console c++14
QT -= gui core

HEADERS += cfg.hpp

SOURCES += cfg.cpp \
           main.cpp

TEST_CFG_FILES += all_is_ok.cfg \
	all_is_ok_with_comments.cfg \
	empty_file.cfg \
	inconsistency_to_int_constraint.cfg \
	inconsistency_to_string_constraint.cfg \
	undefined_child_mandatory_tag.cfg \
	undefined_mandatory_tag.cfg \
	xml.cfg

include( ../../test.pri )

INCLUDEPATH += $$PWD/../../../3rdparty $$PWD/../../..
DEPENDPATH += $$PWD/../../../3rdparty $$PWD/../../..
