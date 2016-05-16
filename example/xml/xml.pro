
TEMPLATE = app
TARGET = example.xml.app
DESTDIR = .
CONFIG += console
QT -= gui
QT += xml

HEADERS += cfg.hpp

SOURCES += cfg.cpp \
           main.cpp

TEST_CFG_FILES += example.cfg

include( ../example.pri )

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib -lQtConfFile
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib -lQtConfFile
else:symbian: LIBS += -lQtConfFile
else:unix: LIBS += -L$$OUT_PWD/../../lib -lQtConfFile

INCLUDEPATH += $$PWD/../..
DEPENDPATH += $$PWD/../..

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/QtConfFile.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/QtConfFile.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../../lib/libQtConfFile.a
