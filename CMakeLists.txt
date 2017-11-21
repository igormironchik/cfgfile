
cmake_minimum_required( VERSION 2.8.11 )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE "Release"
		CACHE STRING "Choose the type of build."
		FORCE)
endif( NOT CMAKE_BUILD_TYPE )

SET( CMAKE_CXX_STANDARD 14 )

SET( CMAKE_CXX_STANDARD_REQUIRED ON )

file( COPY runtests.rb
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

project( cfgfile )

add_library( cfgfile INTERFACE )

target_include_directories( cfgfile INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )

set( cfgfile_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE )

add_subdirectory( generator )
add_subdirectory( example )

enable_testing()

add_subdirectory( tests )
