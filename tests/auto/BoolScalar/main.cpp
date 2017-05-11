
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

#include "cfg.hpp"

// C++ include.
#include <fstream>

// UnitTest include.
#include <UnitTest/unit_test.hpp>


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
	CHECK_CONDITION( cfg.m_withTrue == true )
	CHECK_CONDITION( cfg.m_withFalse == false )
}

TEST( BoolScalar, testAllIsOkWithTrueAndFalse )
{
	Configuration cfg = loadConfig( "all_is_ok_with_true_and_false.cfg" );

	checkConfig( cfg );
} // testAllIsOkWithTrueAndFalse

TEST( BoolScalar, testAllIsOkWithOnAndOff )
{
	Configuration cfg = loadConfig( "all_is_ok_with_on_and_off.cfg" );

	checkConfig( cfg );
} // testAllIsOkWithOnAndOff

TEST( BoolScalar, testAllIsOkWith1And0 )
{
	Configuration cfg = loadConfig( "all_is_ok_with_1_and_0.cfg" );

	checkConfig( cfg );
} // testAllIsOkWith1And0

TEST( BoolScalar, testFalseInsteadOfTrue )
{
	Configuration cfg = loadConfig( "false_instead_of_true.cfg" );

	CHECK_CONDITION( cfg.m_withTrue == false )
	CHECK_CONDITION( cfg.m_withFalse == false )
} // testFalseInsteadOfTrue

TEST( BoolScalar, testInvalidValue )
{
	CHECK_THROW( loadConfig( "invalid_value.cfg" ), cfgfile::exception_t<> );
} // testInvalidValue

int main()
{
	RUN_ALL_TESTS()

	return 0;
}
