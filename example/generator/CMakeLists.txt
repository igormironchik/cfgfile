
project( example.generator )

set( SRC main.cpp
	${CMAKE_CURRENT_BINARY_DIR}/example.hpp )

add_custom_command( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/example.hpp
	PRE_BUILD
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../generator/${CMAKE_CFG_INTDIR}/cfgfile.generator${CMAKE_EXECUTABLE_SUFFIX} -i example.cfgfile -o ${CMAKE_CURRENT_BINARY_DIR}/example.hpp
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../..
	${CMAKE_CURRENT_BINARY_DIR} )

add_executable( example.generator ${SRC} )
