
set( CMAKE_CXX_STANDARD 14 )

set( CMAKE_CXX_STANDARD_REQUIRED ON )

project( cfgfile.generator )

option( USE_INTERNAL_ARGS_PARSER "Use internal args-parser." ON )

set( SRC cfg.hpp
	generator.hpp
	cfg.cpp
	generator.cpp
	main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/Args )

if( USE_INTERNAL_ARGS_PARSER )
	include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/args-parser )
else()
	find_package( args-parser REQUIRED )
endif()

add_executable( cfgfile.generator ${SRC} )

if( NOT USE_INTERNAL_ARGS_PARSER )
	target_link_libraries( cfgfile.generator PRIVATE args-parser::args-parser )
endif()

install( TARGETS cfgfile.generator 
    RUNTIME DESTINATION bin )
