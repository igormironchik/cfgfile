
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// C++ include.
#include <sstream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// doctest include.
#include <doctest/doctest.h>

// cfgfile include.
#include <cfgfile/input_stream.hpp>

using namespace cfgfile;


TEST_CASE( "testInputStream" )
{
	std::stringstream stream( "one\r\rtwo\r\nthree\n" );

	input_stream_t<> in( "test", stream );

	REQUIRE( in.column_number() == 1 );
	REQUIRE( in.line_number() == 1 );
	REQUIRE( in.get() == 'o' );

	REQUIRE( in.column_number() == 2 );
	REQUIRE( in.line_number() == 1 );
	REQUIRE( in.get() == 'n' );

	REQUIRE( in.column_number() == 3 );
	REQUIRE( in.line_number() == 1 );
	REQUIRE( in.get() == 'e' );

	REQUIRE( in.column_number() == 4 );
	REQUIRE( in.line_number() == 1 );
	REQUIRE( in.get() == '\r' );

	REQUIRE( in.column_number() == 1 );
	REQUIRE( in.line_number() == 2 );
	REQUIRE( in.get() == '\r' );

	in.put_back( '\r' );
	in.put_back( '\r' );

	REQUIRE( in.column_number() == 4 );
	REQUIRE( in.line_number() == 1 );
	REQUIRE( in.get() == '\r' );

	REQUIRE( in.column_number() == 1 );
	REQUIRE( in.line_number() == 2 );
	REQUIRE( in.get() == '\r' );

	REQUIRE( in.column_number() == 1 );
	REQUIRE( in.line_number() == 3 );
	REQUIRE( in.get() == 't' );

	REQUIRE( in.column_number() == 2 );
	REQUIRE( in.line_number() == 3 );
	REQUIRE( in.get() == 'w' );

	REQUIRE( in.column_number() == 3 );
	REQUIRE( in.line_number() == 3 );
	REQUIRE( in.get() == 'o' );

	REQUIRE( in.column_number() == 4 );
	REQUIRE( in.line_number() == 3 );
	REQUIRE( in.get() == '\n' );

	REQUIRE( in.column_number() == 1 );
	REQUIRE( in.line_number() == 4 );
	REQUIRE( in.get() == 't' );

	REQUIRE( in.column_number() == 2 );
	REQUIRE( in.line_number() == 4 );
	REQUIRE( in.get() == 'h' );

	REQUIRE( in.column_number() == 3 );
	REQUIRE( in.line_number() == 4 );
	REQUIRE( in.get() == 'r' );

	REQUIRE( in.column_number() == 4 );
	REQUIRE( in.line_number() == 4 );
	REQUIRE( in.get() == 'e' );

	REQUIRE( in.column_number() == 5 );
	REQUIRE( in.line_number() == 4 );
	REQUIRE( in.get() == 'e' );

	REQUIRE( !in.at_end() );

	REQUIRE( in.column_number() == 6 );
	REQUIRE( in.line_number() == 4 );
	REQUIRE( in.get() == '\n' );

	REQUIRE( in.at_end() );

	REQUIRE( in.get() == '\0' );
	REQUIRE( in.get() == '\0' );
	REQUIRE( in.get() == '\0' );

	REQUIRE( in.at_end() );
}
