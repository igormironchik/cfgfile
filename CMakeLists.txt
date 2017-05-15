
cmake_minimum_required( VERSION 2.8.11 )

SET( CMAKE_CXX_STANDARD 14 )

SET( CMAKE_CXX_STANDARD_REQUIRED ON )

project( cfgfile )

add_subdirectory( generator )
add_subdirectory( example )
add_subdirectory( tests )

enable_testing()

add_test( NAME test.bool_scalar
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tests/auto/BoolScalar/test.bool_scalar
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests/auto/BoolScalar )

add_test( NAME test.complex
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tests/auto/Complex/test.complex
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests/auto/Complex )
