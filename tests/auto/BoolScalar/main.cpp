
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#include "cfg.hpp"

// C++ include.
#include <fstream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// doctest include.
#include <doctest/doctest.h>


Configuration loadConfig( const std::string & fileName )
{
	TagConfiguration readTag;

	std::ifstream file( fileName );

	try {
		if( file.good() )
		{
			cfgfile::read_cfgfile( readTag, file, fileName );

			file.close();
		}
	}
	catch( const cfgfile::exception_t<> & )
	{
		file.close();

		throw;
	}

	return readTag.configuration();
}

void checkConfig( const Configuration & cfg )
{
	REQUIRE( cfg.m_withTrue == true );
	REQUIRE( cfg.m_withFalse == false );
}

TEST_CASE( "testAllIsOkWithTrueAndFalse" )
{
	Configuration cfg = loadConfig( "all_is_ok_with_true_and_false.cfg" );

	checkConfig( cfg );
} // testAllIsOkWithTrueAndFalse

TEST_CASE( "testAllIsOkWithOnAndOff" )
{
	Configuration cfg = loadConfig( "all_is_ok_with_on_and_off.cfg" );

	checkConfig( cfg );
} // testAllIsOkWithOnAndOff

TEST_CASE( "testAllIsOkWith1And0" )
{
	Configuration cfg = loadConfig( "all_is_ok_with_1_and_0.cfg" );

	checkConfig( cfg );
} // testAllIsOkWith1And0

TEST_CASE( "testFalseInsteadOfTrue" )
{
	Configuration cfg = loadConfig( "false_instead_of_true.cfg" );

	REQUIRE( cfg.m_withTrue == false );
	REQUIRE( cfg.m_withFalse == false );
} // testFalseInsteadOfTrue

TEST_CASE( "testInvalidValue" )
{
	REQUIRE_THROWS_AS( loadConfig( "invalid_value.cfg" ), cfgfile::exception_t<> );
} // testInvalidValue
