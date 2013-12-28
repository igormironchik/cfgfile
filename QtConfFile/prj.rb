
require 'mxx_ru/cpp/qt5'

Mxx_ru::Cpp::lib_target {

	rtl_mode( Mxx_ru::Cpp::RTL_SHARED )
	rtti_mode( Mxx_ru::Cpp::RTTI_ENABLED )
	threading_mode( Mxx_ru::Cpp::THREADING_MULTI )

	target( "lib/QtConfFile" )
	
	qt = generator( MxxRu::Cpp::Qt5.new( self ) )

	sources_root( "private" ) {
		cpp_source( "input_stream.cpp" )
		cpp_source( "lex.cpp" )
		cpp_source( "parser.cpp" )
		cpp_source( "parser_info.cpp" )
		cpp_source( "string_format.cpp" )
		cpp_source( "tag.cpp" )
	}

	cpp_source( "exceptions.cpp" )
	cpp_source( "tag_no_value.cpp" )
	cpp_source( "utils.cpp" )
}
