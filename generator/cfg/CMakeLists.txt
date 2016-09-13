
set( SRC cfg.hpp cfg.cpp )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../.. )

include_directories( ${Qt5Core_INCLUDE_DIRS}
	${Qt5Xml_INCLUDE_DIRS} )

add_library( QtConfFile.Generator.Cfg STATIC ${SRC} )

set_property( TARGET QtConfFile.Generator.Cfg PROPERTY CXX_STANDARD 14 )
