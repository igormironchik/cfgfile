
cmake_minimum_required( VERSION 2.8.11 )

project( QtConfFile )

if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fPIC" )
endif()

find_package( Qt5Core REQUIRED )
find_package( Qt5Xml REQUIRED )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin )

add_subdirectory( QtConfFile )
add_subdirectory( generator )
