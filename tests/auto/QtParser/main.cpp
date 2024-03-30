
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// doctest include.
#include <doctest/doctest.h>

// cfgfile include.
#include <cfgfile/all.hpp>


TEST_CASE( "test_undefined_first_mandatory_tag" )
{
	QDomDocument doc;

	cfgfile::tag_no_value_t< cfgfile::qstring_trait_t > tag( "cfg", true );

	cfgfile::parser_t< cfgfile::qstring_trait_t > parser( tag, doc );

	try {
		parser.parse( "test_undefined_first_mandatory_tag" );

		REQUIRE( false );
	}
	catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & x )
	{
		REQUIRE( x.desc() == "Unexpected end of file. "
			"Undefined mandatory tag \"cfg\". In file "
			"\"test_undefined_first_mandatory_tag\" on line -1." );
	}
}

TEST_CASE( "test_unexpected_tag" )
{
	QDomDocument doc;
	QString error;
	int line = 0;
	int column = 0;

	doc.setContent( QLatin1String( "<wrong></wrong>" ),
		true, &error, &line, &column );

	cfgfile::tag_no_value_t< cfgfile::qstring_trait_t > tag( "cfg", true );

	cfgfile::parser_t< cfgfile::qstring_trait_t > parser( tag, doc );

	try {
		parser.parse( "test_unexpected_tag" );

		REQUIRE( false );
	}
	catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & x )
	{
		REQUIRE( x.desc() == "Unexpected tag name. "
			"We expected \"cfg\", but we've got \"wrong\". In file "
			"\"test_unexpected_tag\" on line 1." );
	}
}

TEST_CASE( "test_unexpected_child_tag" )
{
	QDomDocument doc;
	QString error;
	int line = 0;
	int column = 0;

	doc.setContent( QLatin1String( "<cfg><wrong></wrong></cfg>" ),
		true, &error, &line, &column );

	cfgfile::tag_no_value_t< cfgfile::qstring_trait_t > tag( "cfg", true );

	cfgfile::parser_t< cfgfile::qstring_trait_t > parser( tag, doc );

	try {
		parser.parse( "test_unexpected_child_tag" );

		REQUIRE( false );
	}
	catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & x )
	{
		REQUIRE( x.desc() == "Unexpected tag name. "
			"We expected one child tag of tag \"cfg\", but we've got \"wrong\". "
			"In file \"test_unexpected_child_tag\" on line 1." );
	}
}

TEST_CASE( "test_cant_set_xml" )
{
	QTextStream s( "<a>abc" );

	cfgfile::tag_no_value_t< cfgfile::qstring_trait_t > tag( "cfg", true );

	try {
		cfgfile::read_cfgfile( tag, s, "test_cant_set_xml" );

		REQUIRE( false );
	}
	catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & x )
	{
		REQUIRE( x.desc() == "Unable to parse XML "
			"from file: \"test_cant_set_xml\". \"Premature end of document.\" "
			"On line 1, column 6." );
	}
}
