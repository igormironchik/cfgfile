
TEMPLATE = subdirs


SUBDIRS = QtConfFile \
          test \
          example

test.depends = QtConfFile
example.depends = QtConfFile

libDir.commands = mkdir lib

QMAKE_EXTRA_TARGETS += libDir
