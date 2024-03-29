
cmake_minimum_required( VERSION 3.19 )

set( CFGFILE_VERSION "0.2.11" )

option( BUILD_EXAMPLES "Build examples? Default ON." ON )
option( BUILD_TESTS "Build tests? Default ON." ON )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE "Release"
		CACHE STRING "Choose the type of build."
		FORCE)
endif( NOT CMAKE_BUILD_TYPE )

SET( CMAKE_CXX_STANDARD 14 )

SET( CMAKE_CXX_STANDARD_REQUIRED ON )

project( cfgfile )

add_subdirectory( generator )

if( ${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME} )

	if( BUILD_EXAMPLES )
		add_subdirectory( example )
	endif()

	if( BUILD_TESTS )
		enable_testing()

		add_subdirectory( tests )
	endif()

	file( GLOB_RECURSE SRC cfgfile/* )
	
	add_library( cfgfile INTERFACE ${SRC} )
	add_library( cfgfile::cfgfile ALIAS cfgfile )

	target_include_directories( cfgfile INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

	install( DIRECTORY cfgfile
		DESTINATION include
	)

	install( TARGETS cfgfile
		EXPORT cfgfile-targets
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
		INCLUDES DESTINATION include
	)

	install( EXPORT cfgfile-targets
		DESTINATION lib/cmake/cfgfile
		NAMESPACE cfgfile::
	)

	include( CMakePackageConfigHelpers )

	write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/cfgfile-config-version.cmake"
		VERSION ${CFGFILE_VERSION}
		COMPATIBILITY AnyNewerVersion
	)

	configure_package_config_file( cfgfile-config.cmake.in
		"${CMAKE_CURRENT_BINARY_DIR}/cfgfile-config.cmake"
		INSTALL_DESTINATION lib/cmake/cfgfile )

	install( FILES ${PROJECT_BINARY_DIR}/cfgfile-config.cmake
			${PROJECT_BINARY_DIR}/cfgfile-config-version.cmake
		DESTINATION lib/cmake/cfgfile
	)
else()

	set( cfgfile_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE )

endif( ${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME} )

