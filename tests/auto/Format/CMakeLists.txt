
project( test.format )

set( SRC main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../.. )

add_executable( test.format ${SRC} )

find_package( Qt5Core )

target_link_libraries( test.format Qt5::Core )

target_compile_definitions( test.format PRIVATE CFGFILE_QT_SUPPORT )

add_test( NAME test.format
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.format
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
