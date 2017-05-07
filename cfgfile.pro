
TEMPLATE = subdirs

SUBDIRS = generator \
	example

example.depends = generator
