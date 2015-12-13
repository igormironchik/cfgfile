
TEMPLATE = app
DESTDIR = .
CONFIG += console
QT += testlib
QT -= gui

HEADERS += cfg.hpp

SOURCES += cfg.cpp \
           main.cpp

TEST_CFG_FILES += all_is_ok_with_1_and_0.cfg \
			   all_is_ok_with_on_and_off.cfg \
			   all_is_ok_with_true_and_false.cfg \
			   false_instead_of_true.cfg \
			   invalid_value.cfg

include( ../test.pri)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib -lQtConfFile
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib -lQtConfFile
else:symbian: LIBS += -lQtConfFile
else:unix: LIBS += -L$$OUT_PWD/../../lib -lQtConfFile

INCLUDEPATH += $$PWD/../..
DEPENDPATH += $$PWD/../..

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/QtConfFile.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/QtConfFile.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../../lib/libQtConfFile.a
