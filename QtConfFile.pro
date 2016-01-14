
TEMPLATE = subdirs


SUBDIRS = QtConfFile \
          test \
          example \
          generator

test.depends = QtConfFile
example.depends = QtConfFile generator
generator.depends = QtConfFile

libDir.commands = mkdir lib

QMAKE_EXTRA_TARGETS += libDir
