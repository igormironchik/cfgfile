
TEMPLATE = subdirs

SUBDIRS = cfg \
          generator

generator.depends = cfg

OTHER_FILES += README
