
TEMPLATE = app
DESTDIR = .
CONFIG += console
QT -= gui core

HEADERS += cfg.hpp

SOURCES += cfg.cpp \
           main.cpp

TEST_CFG_FILES += all_is_ok_with_1_and_0.cfg \
			   all_is_ok_with_on_and_off.cfg \
			   all_is_ok_with_true_and_false.cfg \
			   false_instead_of_true.cfg \
			   invalid_value.cfg

include( ../../test.pri)

INCLUDEPATH += $$PWD/../../..
DEPENDPATH += $$PWD/../../..
