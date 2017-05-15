
project( example.simple )

set( SRC main.cpp
	cfg.hpp
	cfg.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../.. )

add_executable( example.simple ${SRC} )
