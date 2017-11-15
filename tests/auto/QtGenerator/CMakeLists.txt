
project( test.qt_generator )

if( ENABLE_COVERAGE )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage" )
endif( ENABLE_COVERAGE )

set( SRC main.cpp
	custom.hpp
	${CMAKE_CURRENT_BINARY_DIR}/test.hpp )

file( COPY test.cfg
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

file( COPY test1.cfg
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

file( COPY test2.cfg
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

add_executable( test.qt_generator ${SRC} )
	
find_package( Qt5Core )
find_package( Qt5Xml )

target_link_libraries( test.qt_generator Qt5::Core Qt5::Xml )

target_compile_definitions( test.qt_generator PRIVATE CFGFILE_QT_SUPPORT CFGFILE_XML_SUPPORT )

add_test( NAME test.qt_generator
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.qt_generator
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
