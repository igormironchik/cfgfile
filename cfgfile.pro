
TEMPLATE = subdirs

SUBDIRS = generator \
	example \
	tests

example.depends = generator

OTHER_FILES = runtests.rb \
	README.md
