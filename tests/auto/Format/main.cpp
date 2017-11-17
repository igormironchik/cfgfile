
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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// doctest include.
#include <doctest/doctest.h>

// cfgfile include.
#include <cfgfile/all.hpp>

// C++ include.
#include <cmath>


using namespace cfgfile;


TEST_CASE( "allIsOk" )
{
	parser_info_t< string_trait_t > ps( "test.cfg", 1, 1 );
	parser_info_t< wstring_trait_t > pw( L"test.cfg", 1, 1 );
	parser_info_t< qstring_trait_t > pq( "test.cfg", 1, 1 );

	// int
	{
		const auto str =
			format_t< int, string_trait_t >::to_string(
				std::numeric_limits< int >::max() );

		REQUIRE(
			( format_t< int, string_trait_t >::from_string( ps, str ) ==
				std::numeric_limits< int >::max()) );
	}

	{
		const auto str =
			format_t< int, wstring_trait_t >::to_string(
				std::numeric_limits< int >::max() );

		REQUIRE(
			( format_t< int, wstring_trait_t >::from_string( pw, str ) ==
				std::numeric_limits< int >::max() ) );
	}

	{
		const auto str =
			format_t< int, qstring_trait_t >::to_string(
				std::numeric_limits< int >::max() );

		REQUIRE(
			( format_t< int, qstring_trait_t >::from_string( pq, str ) ==
				std::numeric_limits< int >::max() ) );
	}

	// unsigned int
	{
		const auto str =
			format_t< unsigned int, string_trait_t >::to_string(
				std::numeric_limits< unsigned int >::max() );

		REQUIRE(
			( format_t< unsigned int, string_trait_t >::from_string( ps, str ) ==
				std::numeric_limits< unsigned int >::max() ) );
	}

	{
		const auto str =
			format_t< unsigned int, wstring_trait_t >::to_string(
				std::numeric_limits< unsigned int >::max() );

		REQUIRE(
			( format_t< unsigned int, wstring_trait_t >::from_string( pw, str ) ==
				std::numeric_limits< unsigned int >::max() ) );
	}

	{
		const auto str =
			format_t< unsigned int, qstring_trait_t >::to_string(
				std::numeric_limits< unsigned int >::max() );

		REQUIRE(
			( format_t< unsigned int, qstring_trait_t >::from_string( pq, str ) ==
				std::numeric_limits< unsigned int >::max() ) );
	}

	// long
	{
		const auto str =
			format_t< long, string_trait_t >::to_string(
				std::numeric_limits< long >::max() );

		REQUIRE(
			( format_t< long, string_trait_t >::from_string( ps, str ) ==
				std::numeric_limits< long >::max() ) );
	}

	{
		const auto str =
			format_t< long, wstring_trait_t >::to_string(
				std::numeric_limits< long >::max() );

		REQUIRE(
			( format_t< long, wstring_trait_t >::from_string( pw, str ) ==
				std::numeric_limits< long >::max() ) );
	}

	{
		const auto str =
			format_t< long, qstring_trait_t >::to_string(
				std::numeric_limits< long >::max() );

		REQUIRE(
			( format_t< long, qstring_trait_t >::from_string( pq, str ) ==
				std::numeric_limits< long >::max() ) );
	}

	// unsigned long
	{
		const auto str =
			format_t< unsigned long, string_trait_t >::to_string(
				std::numeric_limits< unsigned long >::max() );

		REQUIRE(
			( format_t< unsigned long, string_trait_t >::from_string( ps, str ) ==
				std::numeric_limits< unsigned long >::max() ) );
	}

	{
		const auto str =
			format_t< unsigned long, wstring_trait_t >::to_string(
				std::numeric_limits< unsigned long >::max() );

		REQUIRE(
			( format_t< unsigned long, wstring_trait_t >::from_string( pw, str ) ==
				std::numeric_limits< unsigned long >::max() ) );
	}

	{
		const auto str =
			format_t< unsigned long, qstring_trait_t >::to_string(
				std::numeric_limits< unsigned long >::max() );

		REQUIRE(
			( format_t< unsigned long, qstring_trait_t >::from_string( pq, str ) ==
				std::numeric_limits< unsigned long >::max() ) );
	}

	// long long
	{
		const auto str =
			format_t< long long, string_trait_t >::to_string(
				std::numeric_limits< long long >::max() );

		REQUIRE(
			( format_t< long long, string_trait_t >::from_string( ps, str ) ==
				std::numeric_limits< long long >::max() ) );
	}

	{
		const auto str =
			format_t< long long, wstring_trait_t >::to_string(
				std::numeric_limits< long long >::max() );

		REQUIRE(
			( format_t< long long, wstring_trait_t >::from_string( pw, str ) ==
				std::numeric_limits< long long >::max() ) );
	}

	{
		const auto str =
			format_t< long long, qstring_trait_t >::to_string(
				std::numeric_limits< long long >::max() );

		REQUIRE(
			( format_t< long long, qstring_trait_t >::from_string( pq, str ) ==
				std::numeric_limits< long long >::max() ) );
	}

	// unsigned long long
	{
		const auto str =
			format_t< unsigned long long, string_trait_t >::to_string(
				std::numeric_limits< unsigned long long >::max() );

		REQUIRE(
			( format_t< unsigned long long, string_trait_t >::from_string( ps, str ) ==
				std::numeric_limits< unsigned long long >::max() ) );
	}

	{
		const auto str =
			format_t< unsigned long long, wstring_trait_t >::to_string(
				std::numeric_limits< unsigned long long >::max() );

		REQUIRE(
			( format_t< unsigned long long, wstring_trait_t >::from_string( pw, str ) ==
				std::numeric_limits< unsigned long long >::max() ) );
	}

	{
		const auto str =
			format_t< unsigned long long, qstring_trait_t >::to_string(
				std::numeric_limits< unsigned long long >::max() );

		REQUIRE(
			( format_t< unsigned long long, qstring_trait_t >::from_string( pq, str ) ==
				std::numeric_limits< unsigned long long >::max() ) );
	}

	// double
	{
		const auto str =
			format_t< double, string_trait_t >::to_string(
				1234.9876 );

		REQUIRE(
			( std::abs( format_t< double, string_trait_t >::from_string( ps, str ) -
				1234.9876 ) < 0.00001 ) );
	}

	{
		const auto str =
			format_t< double, wstring_trait_t >::to_string(
				1234.9876 );

		REQUIRE(
			( std::abs( format_t< double, wstring_trait_t >::from_string( pw, str ) -
				1234.9876 ) < 0.00001 ) );
	}

	{
		const auto str =
			format_t< double, qstring_trait_t >::to_string(
				1234.9876 );

		REQUIRE(
			( std::abs( format_t< double, qstring_trait_t >::from_string( pq, str ) -
				1234.9876 ) < 0.00001 ) );
	}

	// std::string
	{
		const auto str =
			format_t< std::string, string_trait_t >::to_string(
				"1234.9876" );

		REQUIRE(
			( format_t< std::string, string_trait_t >::from_string( ps, str ) ==
				"1234.9876" ) );
	}

	{
		const auto str =
			format_t< std::string, wstring_trait_t >::to_string(
				"1234.9876" );

		REQUIRE(
			( format_t< std::string, wstring_trait_t >::from_string( pw, str ) ==
				"1234.9876" ) );
	}

	{
		const auto str =
			format_t< std::string, qstring_trait_t >::to_string(
				"1234.9876" );

		REQUIRE(
			( format_t< std::string, qstring_trait_t >::from_string( pq, str ) ==
				"1234.9876" ) );
	}

	// std::wstring
	{
		const auto str =
			format_t< std::wstring, string_trait_t >::to_string(
				L"1234.9876" );

		REQUIRE(
			( format_t< std::wstring, string_trait_t >::from_string( ps, str ) ==
				L"1234.9876" ) );
	}

	{
		const auto str =
			format_t< std::wstring, wstring_trait_t >::to_string(
				L"1234.9876" );

		REQUIRE(
			( format_t< std::wstring, wstring_trait_t >::from_string( pw, str ) ==
				L"1234.9876" ) );
	}

	{
		const auto str =
			format_t< std::wstring, qstring_trait_t >::to_string(
				L"1234.9876" );

		REQUIRE(
			( format_t< std::wstring, qstring_trait_t >::from_string( pq, str ) ==
				L"1234.9876" ) );
	}

	// QString
	{
		const auto str =
			format_t< QString, string_trait_t >::to_string(
				"1234.9876" );

		REQUIRE(
			( format_t< QString, string_trait_t >::from_string( ps, str ) ==
				"1234.9876" ) );
	}

	{
		const auto str =
			format_t< QString, wstring_trait_t >::to_string(
				"1234.9876" );

		REQUIRE(
			( format_t< QString, wstring_trait_t >::from_string( pw, str ) ==
				"1234.9876" ) );
	}

	{
		const auto str =
			format_t< QString, qstring_trait_t >::to_string(
				"1234.9876" );

		REQUIRE(
			( format_t< QString, qstring_trait_t >::from_string( pq, str ) ==
				"1234.9876" ) );
	}

	// bool
	{
		const auto str =
			format_t< bool, string_trait_t >::to_string( true );

		REQUIRE(
			( format_t< bool, string_trait_t >::from_string( ps, str ) ==
				true ) );
	}

	{
		const auto str =
			format_t< bool, string_trait_t >::to_string( false );

		REQUIRE(
			( format_t< bool, string_trait_t >::from_string( ps, str ) ==
				false ) );
	}
} // allIsOk

TEST_CASE( "Failed" )
{
	parser_info_t< string_trait_t > ps( "test.cfg", 1, 1 );

	REQUIRE_THROWS_AS( ( format_t< int, string_trait_t >::from_string( ps,
		"9999999999999999999999999999999999" ) ),
			exception_t< string_trait_t > );
} // Failed

TEST_CASE( "test_traits" )
{
	REQUIRE( wstring_trait_t::from_ascii( "abc" ) ==
		L"abc" );

	REQUIRE( qstring_wrapper_t( "str1" ) + qstring_wrapper_t( "str2" ) ==
		qstring_wrapper_t( "str1str2" ) );

	REQUIRE( qstring_trait_t::to_string( 123 ) ==
		qstring_wrapper_t( "123" ) );

	REQUIRE( qstring_trait_t::from_ascii( "abc" ) ==
		qstring_wrapper_t( "abc" ) );
}

TEST_CASE( "test_exceptions" )
{
	parser_info_t< string_trait_t > ps( "test.cfg", 1, 1 );
	parser_info_t< wstring_trait_t > pw( L"test.cfg", 1, 1 );
	parser_info_t< qstring_trait_t > pq( "test.cfg", 1, 1 );

	REQUIRE_THROWS_AS( ( format_t< int, string_trait_t >::from_string( ps,
		"999 999" ) ),
		exception_t< string_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< int, wstring_trait_t >::from_string( pw,
		L"999 999" ) ),
		exception_t< wstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< int, qstring_trait_t >::from_string( pq,
		"999 999" ) ),
		exception_t< qstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< int, string_trait_t >::from_string( ps,
		"999999999999999999999999999999999" ) ),
		exception_t< string_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< int, wstring_trait_t >::from_string( pw,
		L"999999999999999999999999999999999" ) ),
		exception_t< wstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< int, qstring_trait_t >::from_string( pq,
		"999999999999999999999999999999999" ) ),
		exception_t< qstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< unsigned int, string_trait_t >::from_string( ps,
		"999 999" ) ),
		exception_t< string_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< unsigned int, wstring_trait_t >::from_string( pw,
		L"999 999" ) ),
		exception_t< wstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< unsigned int, qstring_trait_t >::from_string( pq,
		"999 999" ) ),
		exception_t< qstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< unsigned int, string_trait_t >::from_string( ps,
		"999999999999999999999999999999999" ) ),
		exception_t< string_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< unsigned int, wstring_trait_t >::from_string( pw,
		L"999999999999999999999999999999999" ) ),
		exception_t< wstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< unsigned int, qstring_trait_t >::from_string( pq,
		"999999999999999999999999999999999" ) ),
		exception_t< qstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< long, string_trait_t >::from_string( ps,
		"999 999" ) ),
		exception_t< string_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< long, wstring_trait_t >::from_string( pw,
		L"999 999" ) ),
		exception_t< wstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< long, qstring_trait_t >::from_string( pq,
		"999 999" ) ),
		exception_t< qstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< long, string_trait_t >::from_string( ps,
		"999999999999999999999999999999999" ) ),
		exception_t< string_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< long, wstring_trait_t >::from_string( pw,
		L"999999999999999999999999999999999" ) ),
		exception_t< wstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< long, qstring_trait_t >::from_string( pq,
		"999999999999999999999999999999999" ) ),
		exception_t< qstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< unsigned long, string_trait_t >::from_string( ps,
		"999 999" ) ),
		exception_t< string_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< unsigned long, wstring_trait_t >::from_string( pw,
		L"999 999" ) ),
		exception_t< wstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< unsigned long, qstring_trait_t >::from_string( pq,
		"999 999" ) ),
		exception_t< qstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< unsigned long, string_trait_t >::from_string( ps,
		"999999999999999999999999999999999" ) ),
		exception_t< string_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< unsigned long, wstring_trait_t >::from_string( pw,
		L"999999999999999999999999999999999" ) ),
		exception_t< wstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< unsigned long, qstring_trait_t >::from_string( pq,
		"999999999999999999999999999999999" ) ),
		exception_t< qstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< long long, string_trait_t >::from_string( ps,
		"999 999" ) ),
		exception_t< string_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< long long, wstring_trait_t >::from_string( pw,
		L"999 999" ) ),
		exception_t< wstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< long long, qstring_trait_t >::from_string( pq,
		"999 999" ) ),
		exception_t< qstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< long long, string_trait_t >::from_string( ps,
		"999999999999999999999999999999999" ) ),
		exception_t< string_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< long long, wstring_trait_t >::from_string( pw,
		L"999999999999999999999999999999999" ) ),
		exception_t< wstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< long long, qstring_trait_t >::from_string( pq,
		"999999999999999999999999999999999" ) ),
		exception_t< qstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< unsigned long long, string_trait_t >::from_string( ps,
		"999 999" ) ),
		exception_t< string_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< unsigned long long, wstring_trait_t >::from_string( pw,
		L"999 999" ) ),
		exception_t< wstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< unsigned long long, qstring_trait_t >::from_string( pq,
		"999 999" ) ),
		exception_t< qstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< unsigned long long, string_trait_t >::from_string( ps,
		"999999999999999999999999999999999" ) ),
		exception_t< string_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< unsigned long long, wstring_trait_t >::from_string( pw,
		L"999999999999999999999999999999999" ) ),
		exception_t< wstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< unsigned long long, qstring_trait_t >::from_string( pq,
		"999999999999999999999999999999999" ) ),
		exception_t< qstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< double, string_trait_t >::from_string( ps,
		"999 999" ) ),
		exception_t< string_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< double, wstring_trait_t >::from_string( pw,
		L"999 999" ) ),
		exception_t< wstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< double, qstring_trait_t >::from_string( pq,
		"999 999" ) ),
		exception_t< qstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< double, string_trait_t >::from_string( ps,
		"9.9e+999999999999999999999999999999999999999999999999999999999999" ) ),
		exception_t< string_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< double, wstring_trait_t >::from_string( pw,
		L"9.9e+999999999999999999999999999999999999999999999999999999999999" ) ),
		exception_t< wstring_trait_t > );

	REQUIRE_THROWS_AS( ( format_t< double, qstring_trait_t >::from_string( pq,
		"9.9e+999999999999999999999999999999999999999999999999999999999999" ) ),
		exception_t< qstring_trait_t > );
}

TEST_CASE( "test_to_cfg_file_format" )
{
	REQUIRE( to_cfgfile_format< string_trait_t > ( "" ) == "\"\"" );

	REQUIRE( to_cfgfile_format< string_trait_t > ( "\"\n\r\t\\a" ) ==
		"\"\\\"\\n\\r\\t\\\\a\"" );
}

TEST_CASE( "test_from_cfg_file_format" )
{
	REQUIRE( from_cfgfile_format< string_trait_t > (
		to_cfgfile_format< string_trait_t > ( "\"\\\"\n\r\t\\\"" ) ) ==
		"\"\\\"\n\r\t\\\"" );

	REQUIRE( from_cfgfile_format< string_trait_t > (
		to_cfgfile_format< string_trait_t > ( "\\\\n" ) ) == "\\\\n" );

	try {
		from_cfgfile_format< string_trait_t > ( "\"\\o\"" );

		REQUIRE( false );
	}
	catch( const exception_t< string_trait_t > & x )
	{
		REQUIRE( x.desc() == "Unrecognized backslash sequence \"\\o\"." );
	}

	try {
		from_cfgfile_format< string_trait_t > ( "\"\\\"" );

		REQUIRE( false );
	}
	catch( const exception_t< string_trait_t > & x )
	{
		REQUIRE( x.desc() == "Unfinished backslash sequence \"\\\"." );
	}

	REQUIRE( from_cfgfile_format< string_trait_t > ( "abc" ) == "abc" );
}
