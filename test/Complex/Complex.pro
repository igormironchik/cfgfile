
TEMPLATE = app
DESTDIR = .
CONFIG += console
QT += testlib
QT -= gui

HEADERS += cfg.hpp

SOURCES += cfg.cpp \
           main.cpp

TEST_CFG_FILES += all_is_ok.cfg \
			   all_is_ok_with_comments.cfg \
			   empty_file.cfg \
			   inconsistency_to_int_constraint.cfg \
			   inconsistency_to_string_constraint.cfg \
			   undefined_child_mandatory_tag.cfg \
			   undefined_mandatory_tag.cfg

include( ../test.pri )

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib -lQtConfFile
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib -lQtConfFile
else:symbian: LIBS += -lQtConfFile
else:unix: LIBS += -L$$OUT_PWD/../../lib -lQtConfFile

INCLUDEPATH += $$PWD/../..
DEPENDPATH += $$PWD/../..

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/QtConfFile.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/QtConfFile.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../../lib/libQtConfFile.a
