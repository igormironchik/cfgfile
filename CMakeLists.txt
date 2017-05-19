
cmake_minimum_required( VERSION 2.8.11 )

SET( CMAKE_CXX_STANDARD 14 )

SET( CMAKE_CXX_STANDARD_REQUIRED ON )

project( cfgfile )

add_subdirectory( generator )
add_subdirectory( example )

enable_testing()

add_subdirectory( tests )
