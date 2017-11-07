
project( example.xml )

set( SRC main.cpp
	cfg.hpp
	cfg.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../.. )

file( COPY example.cfg
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

find_package( Qt5Core )
find_package( Qt5Xml )

add_executable( example.xml ${SRC} )

target_link_libraries( example.xml Qt5::Core Qt5::Xml )

target_compile_definitions( example.xml PRIVATE
	CFGFILE_QT_SUPPORT CFGFILE_XML_SUPPORT )
