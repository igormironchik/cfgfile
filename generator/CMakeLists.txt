
project( cfgfile.generator )

set( SRC cfg.hpp
	generator.hpp
	cfg.cpp
	generator.cpp
	main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/../Args )

add_executable( cfgfile.generator ${SRC} )

set_property( TARGET cfgfile.generator PROPERTY CXX_STANDARD 14 )
