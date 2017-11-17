
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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// doctest include.
#include <doctest/doctest.h>

// C++ include.
#include <fstream>
#include <sstream>


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
	REQUIRE( cfg.m_stringValue == "string" );
	REQUIRE( cfg.m_listOfStringValues.size() == 3 );
	REQUIRE( cfg.m_listOfStringValues.at( 0 ) == "str1" );
	REQUIRE( cfg.m_listOfStringValues.at( 1 ) == "str2" );
	REQUIRE( cfg.m_listOfStringValues.at( 2 ) == "str3" );
	REQUIRE( cfg.m_intValue == 100 );
	REQUIRE( cfg.m_vectorOfTags.size() == 2 );
	REQUIRE( cfg.m_vectorOfTags.at( 0 ).m_stringValue ==
		"string1" );
	REQUIRE( cfg.m_vectorOfTags.at( 0 ).m_intValue == 100 );
	REQUIRE( cfg.m_vectorOfTags.at( 1 ).m_stringValue ==
		"string2" );
	REQUIRE( cfg.m_vectorOfTags.at( 1 ).m_intValue == 200 );
}

TEST_CASE( "testAllIsOk" )
{
	Configuration cfg = loadConfig( "all_is_ok.cfg" );

	checkConfig( cfg );
} // testAllIsOk

TEST_CASE( "testAllIsOkWithComments" )
{
	Configuration cfg = loadConfig( "all_is_ok_with_comments.cfg" );

	checkConfig( cfg );
} // testAllIsOkWithComments

TEST_CASE( "testIncosistencyToStringConstraint" )
{
	try {
		loadConfig( "inconsistency_to_string_constraint.cfg" );

		REQUIRE( false );
	}
	catch( const cfgfile::exception_t<> & x )
	{
		REQUIRE( "Invalid value: \"str4\". Value must match "
			"to the constraint in tag \"listOfStringValues\". "
			"In file \"inconsistency_to_string_constraint.cfg\" "
			"on line 3." == x.desc() );
	}
} // testIncosistencyToStringConstraint

TEST_CASE( "testIncosistencyToIntConstraint" )
{
	try {
		loadConfig( "inconsistency_to_int_constraint.cfg" );

		REQUIRE( false );
	}
	catch( const cfgfile::exception_t<> & x )
	{
		REQUIRE( "Invalid value: \"200\". Value must match "
			"to the constraint in tag \"intValue\". "
			"In file \"inconsistency_to_int_constraint.cfg\" "
			"on line 4." == x.desc() );
	}
} // testIncosistencyToIntConstraint

TEST_CASE( "testUndefinedChildMandatoryTag" )
{
	try {
		loadConfig( "undefined_child_mandatory_tag.cfg" );

		REQUIRE( false );
	}
	catch( const cfgfile::exception_t<> & x )
	{
		REQUIRE( "Undefined child mandatory tag: \"stringValue\". "
			"Where parent is: \"vecOfTags\". "
			"In file \"undefined_child_mandatory_tag.cfg\" on line 8." ==
			x.desc() );
	}
} // testUndefinedChildMandatoryTag

TEST_CASE( "testUndefinedMandatoryTag" )
{
	try {
		loadConfig( "undefined_mandatory_tag.cfg" );

		REQUIRE( false );
	}
	catch( const cfgfile::exception_t<> & x )
	{
		REQUIRE( "Undefined child mandatory tag: \"stringValue\". "
			"Where parent is: \"cfg\". "
			"In file \"undefined_mandatory_tag.cfg\" on line 14." ==
			x.desc() );
	}
} // testUndefinedMandatoryTag

TEST_CASE( "testEmptyFile" )
{
	try {
		loadConfig( "empty_file.cfg" );

		REQUIRE( false );
	}
	catch( const cfgfile::exception_t<> & x )
	{
		REQUIRE( "Unexpected end of file. Undefined "
			"mandatory tag \"cfg\". In file \"empty_file.cfg\" on line 1." ==
			x.desc() );
	}
} // testEmptyFile

TEST_CASE( "testUnsupportedXML" )
{
	try {
		loadConfig( "xml.cfg" );

		REQUIRE( false );
	}
	catch( const cfgfile::exception_t<> & x )
	{
		REQUIRE( "XML supported only with Qt. Parsing of file "
			"\"xml.cfg\" failed." == x.desc() );
	}
}

TEST_CASE( "testUnsupportedXMLWrite" )
{
	try {
		std::stringstream stream;
		cfgfile::tag_no_value_t<> tag( "cfg" );

		cfgfile::write_cfgfile( tag, stream,
			cfgfile::file_format_t::xml_format );

		REQUIRE( false );
	}
	catch( const cfgfile::exception_t<> & x )
	{
		REQUIRE( "XML supported only with Qt." == x.desc() );
	}
}
