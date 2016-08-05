
TEMPLATE = subdirs


SUBDIRS = QtConfFile \
          tests \
          example \
          generator

tests.depends = QtConfFile
example.depends = QtConfFile generator
generator.depends = QtConfFile

libDir.commands = mkdir lib

QMAKE_EXTRA_TARGETS += libDir
