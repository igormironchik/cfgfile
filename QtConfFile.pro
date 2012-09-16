
TEMPLATE = subdirs


SUBDIRS = QtConfFile \
          test \
          example

test.depends = QtConfFile
example.depends = QtConfFile
