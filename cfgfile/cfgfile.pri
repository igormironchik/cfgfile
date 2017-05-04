

HEADERS += $$PWD/private/constraint.hpp \
           $$PWD/private/format.hpp \
           $$PWD/private/input_stream.hpp \
           $$PWD/private/lex.hpp \
           $$PWD/private/parser.hpp \
           $$PWD/private/parser_info.hpp \
           $$PWD/private/tag.hpp \
           $$PWD/private/string_format.hpp \
           $$PWD/constraint_min_max.hpp \
           $$PWD/constraint_one_of.hpp \
           $$PWD/exceptions.hpp \
           $$PWD/tag_no_value.hpp \
           $$PWD/tag_scalar.hpp \
           $$PWD/tag_scalar_vector.hpp \
           $$PWD/tag_vector_of_tags.hpp \
           $$PWD/utils.hpp

SOURCES += $$PWD/private/input_stream.cpp \
           $$PWD/private/lex.cpp \
           $$PWD/private/parser.cpp \
           $$PWD/private/parser_info.cpp \
           $$PWD/private/tag.cpp \
           $$PWD/private/string_format.cpp \
           $$PWD/exceptions.cpp \
           $$PWD/tag_no_value.cpp \
           $$PWD/utils.cpp
