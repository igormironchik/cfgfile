
project( example.xml )

set( SRC main.cpp
	cfg.hpp
	cfg.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../.. )

file( COPY example.cfg
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

find_package( Qt6Core )
find_package( Qt6Xml )

add_executable( xml.example ${SRC} )

target_link_libraries( xml.example Qt6::Core Qt6::Xml )

target_compile_definitions( xml.example PRIVATE
	CFGFILE_QT_SUPPORT CFGFILE_XML_SUPPORT )
