
TEMPLATE = subdirs

SUBDIRS = generator \
	example \
	tests

example.depends = generator
