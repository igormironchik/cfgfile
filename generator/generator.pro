
TEMPLATE = subdirs

SUBDIRS = cfg \
          generator

generator.depends = cfg
