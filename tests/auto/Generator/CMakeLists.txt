
project( test.generator )

if( ENABLE_COVERAGE )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage" )
endif( ENABLE_COVERAGE )

set( SRC main.cpp
	custom.hpp
	${CMAKE_CURRENT_BINARY_DIR}/test.hpp )

file( COPY test.cfg
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

add_custom_command( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test.hpp
	PRE_BUILD
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../generator/${CMAKE_CFG_INTDIR}/cfgfile.generator${CMAKE_EXECUTABLE_SUFFIX} -i test.cfgfile -o ${CMAKE_CURRENT_BINARY_DIR}/test.hpp
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../..
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty )

add_executable( test.generator ${SRC} )

add_test( NAME test.generator
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.generator
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
