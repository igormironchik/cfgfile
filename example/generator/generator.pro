
TEMPLATE = app
TARGET = example.generator
DESTDIR = .
CONFIG += console c++14
QT -= gui core

SOURCES += main.cpp

TO_GENERATE = example.cfgfile

QMAKE_EXTRA_COMPILERS += generate_cfg
generate_cfg.name = CONF_GEN
generate_cfg.input = TO_GENERATE
generate_cfg.output = ${QMAKE_FILE_BASE}.hpp
generate_cfg.CONFIG = no_link
generate_cfg.variable_out = HEADERS

generate_cfg.commands = $$system_path( $$absolute_path( $${OUT_PWD}/../../cfgfile.generator ) ) \
-i ${QMAKE_FILE_IN} \
-o $${OUT_PWD}/${QMAKE_FILE_BASE}.hpp

PRE_TARGETDEPS += compiler_generate_cfg_make_all

INCLUDEPATH += $$PWD/../..
DEPENDPATH += $$PWD/../..
