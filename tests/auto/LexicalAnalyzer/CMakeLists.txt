
project( test.lexical_analyzer )

set( SRC main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../.. )

add_executable( test.lexical_analyzer ${SRC} )

add_test( NAME test.lexical_analyzer
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.lexical_analyzer
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
