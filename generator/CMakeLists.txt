
project( cfgfile.generator )

set( SRC cfg.hpp
	generator.hpp
	cfg.cpp
	generator.cpp
	main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/Args )

add_executable( cfgfile.generator ${SRC} )
