
TEMPLATE = app
DESTDIR = .
CONFIG += console c++14
QT -= gui
QT += core xml
DEFINES += CFGFILE_QT_SUPPORT CFGFILE_XML_SUPPORT

HEADERS += custom.hpp

SOURCES += main.cpp

TEST_CFG_FILES += test.cfg test1.cfg test2.cfg

include( ../../test.pri )

INCLUDEPATH += $$PWD/../../../3rdparty $$PWD/../../..
DEPENDPATH += $$PWD/../../../3rdparty $$PWD/../../..

TO_GENERATE = test.cfgfile

QMAKE_EXTRA_COMPILERS += generate_cfg
generate_cfg.name = CONF_GEN
generate_cfg.input = TO_GENERATE
generate_cfg.output = ${QMAKE_FILE_BASE}.hpp
generate_cfg.CONFIG = no_link
generate_cfg.variable_out = HEADERS

generate_cfg.commands = $$shell_path( $$absolute_path( $${OUT_PWD}/../../../cfgfile.generator ) ) \
-i ${QMAKE_FILE_IN} \
-o $${OUT_PWD}/${QMAKE_FILE_BASE}.hpp

PRE_TARGETDEPS += compiler_generate_cfg_make_all
