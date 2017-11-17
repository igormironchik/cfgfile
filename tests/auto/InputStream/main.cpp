
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
}
