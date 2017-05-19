
project( test.parser )

set( SRC main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../.. )

add_executable( test.parser ${SRC} )

add_test( NAME test.parser
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.parser
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
