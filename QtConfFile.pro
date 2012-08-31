
TEMPLATE = subdirs


SUBDIRS = QtConfFile \
          test

test.depends = QtConfFile
