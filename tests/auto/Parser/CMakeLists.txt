
project( test.parser )

if( ENABLE_COVERAGE )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage" )
endif( ENABLE_COVERAGE )

set( SRC main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../..
	${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty )

add_executable( test.parser ${SRC} )

find_package( Qt5Core )

target_link_libraries( test.parser Qt5::Core )

target_compile_definitions( test.parser PRIVATE CFGFILE_QT_SUPPORT )

add_test( NAME test.parser
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.parser
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
