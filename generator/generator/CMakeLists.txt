
set( SRC generator.hpp
	generator.cpp
	main.cpp )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../../QtArg
	${CMAKE_CURRENT_SOURCE_DIR}/../.. )

include_directories( ${Qt5Core_INCLUDE_DIRS}
	${Qt5Xml_INCLUDE_DIRS} )

add_executable( qtconffile.generator ${SRC} )

target_link_libraries( qtconffile.generator
	QtConfFile.Generator.Cfg
	QtConfFile
	${Qt5Xml_LIBRARIES}
	${Qt5Core_LIBRARIES} )

set_property( TARGET qtconffile.generator PROPERTY CXX_STANDARD 14 )
