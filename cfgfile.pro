
TEMPLATE = subdirs

SUBDIRS = generator \
	example \
	tests

example.depends = generator

OTHER_FILES = runtests.rb \
	README.md \
	doc/dsl.dox \
	doc/mainpage.dox \
	.travis.yml \
	doc/Doxyfile \
	cfgfile-config.cmake.in

include( cfgfile/cfgfile.pri )
