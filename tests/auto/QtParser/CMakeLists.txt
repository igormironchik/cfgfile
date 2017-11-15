
project( test.qt_parser )

if( ENABLE_COVERAGE )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage" )
endif( ENABLE_COVERAGE )

set( SRC main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../..
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty )

add_executable( test.qt_parser ${SRC} )
	
find_package( Qt5Core )
find_package( Qt5Xml )

target_link_libraries( test.qt_parser Qt5::Core Qt5::Xml )

target_compile_definitions( test.qt_parser PRIVATE CFGFILE_QT_SUPPORT CFGFILE_XML_SUPPORT )

add_test( NAME test.qt_parser
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.qt_parser
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
