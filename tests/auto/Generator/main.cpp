
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2017 Igor Mironchik

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

// C++ include.
#include <fstream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// doctest include.
#include <doctest/doctest.h>

// test include.
#include "test.hpp"


cfg::vector_t load_config( const std::string & file_name )
{
	cfg::tag_vector_t< cfgfile::string_trait_t > read_tag;

	std::ifstream file( file_name );

	try {
		if( file.good() )
		{
			cfgfile::read_cfgfile( read_tag, file, file_name );

			file.close();
		}
	}
	catch( const cfgfile::exception_t<> & )
	{
		file.close();

		throw;
	}

	return read_tag.get_cfg();
}

const std::string c_dummy_file_name = "dummy.cfg";

void write_config( const cfg::vector_t & cfg )
{
	cfg::tag_vector_t< cfgfile::string_trait_t > write_tag( cfg );

	std::ofstream file( c_dummy_file_name );

	try {
		if( file.good() )
		{
			cfgfile::write_cfgfile( write_tag, file );

			file.close();
		}
	}
	catch( const cfgfile::exception_t<> & )
	{
		file.close();

		throw;
	}
}

void check_config( const cfg::vector_t & cfg )
{
	REQUIRE( cfg.vector().size() == 1 );
	REQUIRE( cfg.vector().at( 0 ).string_field() == "one" );
	REQUIRE( cfg.vector().at( 0 ).no_value_field() == true );
	REQUIRE( cfg.vector().at( 0 ).int_field().size() == 2 );
	REQUIRE( cfg.vector().at( 0 ).int_field().at( 0 ) == 100 );
	REQUIRE( cfg.vector().at( 0 ).int_field().at( 1 ) == 200 );
	REQUIRE( cfg.vector().at( 0 ).custom_field().m_value == 300 );

	REQUIRE( cfg.vector().at( 0 ).bool_scalar().bool_scalar() == true );
	REQUIRE( cfg.vector().at( 0 ).bool_scalar().string_field() == "one" );

	REQUIRE( cfg.vector().at( 0 ).int_scalar().int_scalar() == 100 );
	REQUIRE( cfg.vector().at( 0 ).int_scalar().string_field() == "one" );

	REQUIRE( cfg.vector().at( 0 ).string_scalar().string_scalar() == "string" );
	REQUIRE( cfg.vector().at( 0 ).string_scalar().string_field().empty() );

	REQUIRE( cfg.vector().at( 0 ).int_scalar_vector().int_scalar_vector().size() == 3 );
	REQUIRE( cfg.vector().at( 0 ).int_scalar_vector().int_scalar_vector().at( 0 ) == 100 );
	REQUIRE( cfg.vector().at( 0 ).int_scalar_vector().int_scalar_vector().at( 1 ) == 200 );
	REQUIRE( cfg.vector().at( 0 ).int_scalar_vector().int_scalar_vector().at( 2 ) == 300 );
	REQUIRE( cfg.vector().at( 0 ).int_scalar_vector().string_field() ==
		"StringStringStringStringStringStringStringString"
		"StringStringStringStringStringStringStringString"
		"StringStringStringString" );
}


TEST_CASE( "testAllIsOk" )
{
	auto cfg = load_config( "test.cfg" );

	check_config( cfg );

	write_config( cfg );

	cfg = load_config( c_dummy_file_name );

	check_config( cfg );
} // testAllIsOk
