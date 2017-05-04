
TEMPLATE = subdirs

SUBDIRS = tests \
          example \
          generator

example.depends = generator
