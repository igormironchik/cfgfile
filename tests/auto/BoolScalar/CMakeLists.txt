
project( test.bool_scalar )

if( ENABLE_COVERAGE )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage" )
endif( ENABLE_COVERAGE )

set( SRC main.cpp
	cfg.hpp
	cfg.cpp )

file( COPY all_is_ok_with_1_and_0.cfg
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY all_is_ok_with_on_and_off.cfg
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY all_is_ok_with_true_and_false.cfg
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY false_instead_of_true.cfg
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY invalid_value.cfg
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../..
	${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty )

add_executable( test.bool_scalar ${SRC} )

add_test( NAME test.bool_scalar
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.bool_scalar
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
