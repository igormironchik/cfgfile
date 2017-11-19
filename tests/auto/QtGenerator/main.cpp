
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

// Qt include.
#include <QTextStream>
#include <QFile>

#include <QDebug>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// doctest include.
#include <doctest/doctest.h>

// test include.
#include "test.hpp"

// cfgfile include.
#include <cfgfile/all.hpp>


cfg::vector_t load_config( const QString & file_name )
{
	cfg::tag_vector_t< cfgfile::qstring_trait_t > read_tag;

	QFile file( file_name );

	if( !file.open( QIODevice::ReadOnly ) )
		return cfg::vector_t();

	QTextStream stream( &file );

	try {
		cfgfile::read_cfgfile(
			read_tag, stream, file_name );

		file.close();
	}
	catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & )
	{
		file.close();

		throw;
	}

	return read_tag.get_cfg();
}

const QString c_dummy_file_name_1 = QLatin1String( "dummy_1.cfg" );
const QString c_dummy_file_name_2 = QLatin1String( "dummy_2.cfg" );

void write_config( const cfg::vector_t & cfg, const QString & file_name,
	cfgfile::file_format_t fmt )
{
	cfg::tag_vector_t< cfgfile::qstring_trait_t > write_tag( cfg );

	QFile file( file_name );

	if( !file.open( QIODevice::WriteOnly ) )
		return;

	QTextStream stream( &file );

	try {
		cfgfile::write_cfgfile( write_tag, stream, fmt );

		file.close();
	}
	catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & )
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
	REQUIRE( cfg.vector().at( 0 ).int_scalar().string_field() == "one\n\"\r\t\\" );

	REQUIRE( cfg.vector().at( 0 ).string_scalar().string_scalar() == "string" );
	REQUIRE( cfg.vector().at( 0 ).string_scalar().string_field().isEmpty() );

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

	write_config( cfg, c_dummy_file_name_1,
		cfgfile::file_format_t::xml_format );

	cfg = load_config( c_dummy_file_name_1 );

	check_config( cfg );

	write_config( cfg, c_dummy_file_name_2,
		cfgfile::file_format_t::cfgfile_format );

	cfg = load_config( c_dummy_file_name_2 );

	check_config( cfg );
} // testAllIsOk

TEST_CASE( "test_wrong_backslash_1" )
{
	try {
		load_config( "test1.cfg" );

		REQUIRE( false );
	}
	catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & x )
	{
		REQUIRE( x.desc() == "Unrecognized backslash sequence \"\\1\"."
			" In file \"test1.cfg\" on line -1." );
	}
}

TEST_CASE( "test_wrong_backslash_2" )
{
	try {
		load_config( "test2.cfg" );

		REQUIRE( false );
	}
	catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & x )
	{
		REQUIRE( x.desc() == "Unfinished backslash sequence \"\\\"."
			" In file \"test2.cfg\" on line 3." );
	}
}
