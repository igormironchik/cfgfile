
TEMPLATE = app
TARGET = example.generator
DESTDIR = .
CONFIG += console
QT -= gui
QT += xml

SOURCES += main.cpp

TO_GENERATE = example.qtconffile

QMAKE_EXTRA_COMPILERS += generate_cfg
generate_cfg.name = CONF_GEN
generate_cfg.input = TO_GENERATE
generate_cfg.output = ${QMAKE_FILE_BASE}.hpp
generate_cfg.CONFIG = no_link
generate_cfg.variable_out = HEADERS

generate_cfg.commands = $$shell_path( $$absolute_path( $${OUT_PWD}/../../qtconffile.generator ) ) \
-i ${QMAKE_FILE_IN} \
-o $${OUT_PWD}/${QMAKE_FILE_BASE}.hpp

PRE_TARGETDEPS += compiler_generate_cfg_make_all

INCLUDEPATH += $$PWD/../..
DEPENDPATH += $$PWD/../..

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/QtConfFile.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/QtConfFile.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../../lib/libQtConfFile.a
