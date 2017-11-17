
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
#include <cfgfile/lex.hpp>
#include <cfgfile/input_stream.hpp>

// C++ include.
#include <sstream>


TEST_CASE( "test_quotedLexeme1" )
{
	std::stringstream stream( "{firstTag \"lexeme\"}" );

	cfgfile::input_stream_t<> input( "test_quotedLexeme1", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	cfgfile::lexeme_t<> lex1 = analyzer.next_lexeme();
	REQUIRE( lex1.type() == cfgfile::lexeme_type_t::start );

	cfgfile::lexeme_t<> lex2 = analyzer.next_lexeme();
	REQUIRE( lex2.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex2.value() == "firstTag" );

	cfgfile::lexeme_t<> lex3 = analyzer.next_lexeme();
	REQUIRE( lex3.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex3.value() == "lexeme" );

	cfgfile::lexeme_t<> lex4 = analyzer.next_lexeme();
	REQUIRE( lex4.type() == cfgfile::lexeme_type_t::finish );

	REQUIRE( input.at_end() == true );
}

TEST_CASE( "test_quotedLexeme2" )
{
	std::stringstream stream( " \r\n\t {firstTag\"lex eme|\"}" );

	cfgfile::input_stream_t<> input( "test_quotedLexeme2", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	cfgfile::lexeme_t<> lex1 = analyzer.next_lexeme();
	REQUIRE( lex1.type() == cfgfile::lexeme_type_t::start );

	cfgfile::lexeme_t<> lex2 = analyzer.next_lexeme();
	REQUIRE( lex2.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex2.value() == "firstTag" );

	cfgfile::lexeme_t<> lex3 = analyzer.next_lexeme();
	REQUIRE( lex3.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex3.value() == "lex eme|" );

	cfgfile::lexeme_t<> lex4 = analyzer.next_lexeme();
	REQUIRE( lex4.type() == cfgfile::lexeme_type_t::finish );

	REQUIRE( input.at_end() == true );
}

TEST_CASE( "test_quotedLexeme3" )
{
	std::stringstream stream( " \r\n\t {firstTag\"\n\"}" );

	cfgfile::input_stream_t<> input( "test_quotedLexeme2", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	cfgfile::lexeme_t<> lex1 = analyzer.next_lexeme();
	REQUIRE( lex1.type() == cfgfile::lexeme_type_t::start );

	cfgfile::lexeme_t<> lex2 = analyzer.next_lexeme();
	REQUIRE( lex2.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex2.value() == "firstTag" );

	try {
		analyzer.next_lexeme();

		REQUIRE( false );
	}
	catch( const cfgfile::exception_t<> & x )
	{
		REQUIRE( x.desc() == "Unfinished quoted lexeme. "
			"New line detected. In file \"test_quotedLexeme2"
			"\" on line 2." );
	}
}

TEST_CASE( "test_backSlahSequence" )
{
	std::stringstream stream( " \r\n\t {firstTag\"\\n\\r\\t\\\"\\\\lexeme\"}" );

	cfgfile::input_stream_t<> input( "test_backSlahSequence", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	cfgfile::lexeme_t<> lex1 = analyzer.next_lexeme();
	REQUIRE( lex1.type() == cfgfile::lexeme_type_t::start );

	cfgfile::lexeme_t<> lex2 = analyzer.next_lexeme();
	REQUIRE( lex2.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex2.value() == "firstTag" );

	cfgfile::lexeme_t<> lex3 = analyzer.next_lexeme();
	REQUIRE( lex3.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex3.value() == "\n\r\t\"\\lexeme" );

	cfgfile::lexeme_t<> lex4 = analyzer.next_lexeme();
	REQUIRE( lex4.type() == cfgfile::lexeme_type_t::finish );

	REQUIRE( input.at_end() == true );
}

TEST_CASE( "test_wrongBackSlahSequence" )
{
	std::stringstream stream( " \r\n\t {firstTag\"\\glexeme\"}" );

	cfgfile::input_stream_t<> input( "test_wrongBackSlahSequence", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	cfgfile::lexeme_t<> lex1 = analyzer.next_lexeme();
	REQUIRE( lex1.type() == cfgfile::lexeme_type_t::start );

	cfgfile::lexeme_t<> lex2 = analyzer.next_lexeme();
	REQUIRE( lex2.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex2.value() == "firstTag" );

	try {
		cfgfile::lexeme_t<> lex3 = analyzer.next_lexeme();

		REQUIRE( false );
	}
	catch( const cfgfile::exception_t<> & x )
	{
		REQUIRE( x.desc() == "Unrecognized back-slash sequence: \"\\g\". "
			"In file \"test_wrongBackSlahSequence\" on line 2." );
	}
}

TEST_CASE( "test_oneLineComment" )
{
	std::stringstream stream( "{firstTag \"value1\"}\r\n"
		"|| Comment\r\n"
		"{secondTag \"value2\"}\r\n" );

	cfgfile::input_stream_t<> input( "test_oneLineComment", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	cfgfile::lexeme_t<> lex1 = analyzer.next_lexeme();
	REQUIRE( lex1.type() == cfgfile::lexeme_type_t::start );

	cfgfile::lexeme_t<> lex2 = analyzer.next_lexeme();
	REQUIRE( lex2.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex2.value() == "firstTag" );

	cfgfile::lexeme_t<> lex3 = analyzer.next_lexeme();
	REQUIRE( lex3.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex3.value() == "value1" );

	cfgfile::lexeme_t<> lex4 = analyzer.next_lexeme();
	REQUIRE( lex4.type() == cfgfile::lexeme_type_t::finish );

	cfgfile::lexeme_t<> lex5 = analyzer.next_lexeme();
	REQUIRE( lex5.type() == cfgfile::lexeme_type_t::start );

	cfgfile::lexeme_t<> lex6 = analyzer.next_lexeme();
	REQUIRE( lex6.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex6.value() == "secondTag" );

	cfgfile::lexeme_t<> lex7 = analyzer.next_lexeme();
	REQUIRE( lex7.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex7.value() == "value2" );

	cfgfile::lexeme_t<> lex8 = analyzer.next_lexeme();
	REQUIRE( lex8.type() == cfgfile::lexeme_type_t::finish );
}

TEST_CASE( "test_multiLineComment" )
{
	std::stringstream stream( "{firstTag \"value1\"}\r\n"
		"|#\r\n"
		"  Comment\r\n"
		"#|\r\n"
		"{secondTag \"value2\"}\r\n" );

	cfgfile::input_stream_t<> input( "test_multiLineComment", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	cfgfile::lexeme_t<> lex1 = analyzer.next_lexeme();
	REQUIRE( lex1.type() == cfgfile::lexeme_type_t::start );

	cfgfile::lexeme_t<> lex2 = analyzer.next_lexeme();
	REQUIRE( lex2.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex2.value() == "firstTag" );

	cfgfile::lexeme_t<> lex3 = analyzer.next_lexeme();
	REQUIRE( lex3.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex3.value() == "value1" );

	cfgfile::lexeme_t<> lex4 = analyzer.next_lexeme();
	REQUIRE( lex4.type() == cfgfile::lexeme_type_t::finish );

	cfgfile::lexeme_t<> lex5 = analyzer.next_lexeme();
	REQUIRE( lex5.type() == cfgfile::lexeme_type_t::start );

	cfgfile::lexeme_t<> lex6 = analyzer.next_lexeme();
	REQUIRE( lex6.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex6.value() == "secondTag" );

	cfgfile::lexeme_t<> lex7 = analyzer.next_lexeme();
	REQUIRE( lex7.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex7.value() == "value2" );

	cfgfile::lexeme_t<> lex8 = analyzer.next_lexeme();
	REQUIRE( lex8.type() == cfgfile::lexeme_type_t::finish );
}

TEST_CASE( "test_backSlash" )
{
	std::stringstream stream( "{cfg \\n}" );

	cfgfile::input_stream_t<> input( "test_backSlash", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	auto lex1 = analyzer.next_lexeme();
	REQUIRE( lex1.type() == cfgfile::lexeme_type_t::start );

	auto lex2 = analyzer.next_lexeme();
	REQUIRE( lex2.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex2.value() == "cfg" );

	auto lex3 = analyzer.next_lexeme();
	REQUIRE( lex3.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex3.value() == "\\n" );

	auto lex4 = analyzer.next_lexeme();
	REQUIRE( lex4.type() == cfgfile::lexeme_type_t::finish );
}

TEST_CASE( "test_backSlash2" )
{
	std::stringstream stream( "{cfg \"\\" );

	cfgfile::input_stream_t<> input( "test_backSlash2", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	auto lex1 = analyzer.next_lexeme();
	REQUIRE( lex1.type() == cfgfile::lexeme_type_t::start );

	auto lex2 = analyzer.next_lexeme();
	REQUIRE( lex2.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex2.value() == "cfg" );

	try {
		analyzer.next_lexeme();

		REQUIRE( false );
	}
	catch( const cfgfile::exception_t<> & x )
	{
		REQUIRE( x.desc() == "Unexpected end of file. "
			"Unfinished back slash sequence. In file \"test_backSlash2"
			"\" on line 1." );
	}
}

TEST_CASE( "test_unfinishedQuoted" )
{
	std::stringstream stream( "{cfg \"" );

	cfgfile::input_stream_t<> input( "test_unfinishedQuoted", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	auto lex1 = analyzer.next_lexeme();
	REQUIRE( lex1.type() == cfgfile::lexeme_type_t::start );

	auto lex2 = analyzer.next_lexeme();
	REQUIRE( lex2.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex2.value() == "cfg" );

	try {
		analyzer.next_lexeme();

		REQUIRE( false );
	}
	catch( const cfgfile::exception_t<> & x )
	{
		REQUIRE( x.desc() == "Unfinished quoted lexeme. "
			"End of file riched. In file \"test_unfinishedQuoted\" on line 1." );
	}
}

TEST_CASE( "test_startEndTagInString" )
{
	std::stringstream stream( "{cfg \"a{}\"}" );

	cfgfile::input_stream_t<> input( "test_backSlash", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	auto lex1 = analyzer.next_lexeme();
	REQUIRE( lex1.type() == cfgfile::lexeme_type_t::start );

	auto lex2 = analyzer.next_lexeme();
	REQUIRE( lex2.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex2.value() == "cfg" );

	auto lex3 = analyzer.next_lexeme();
	REQUIRE( lex3.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex3.value() == "a{}" );

	auto lex4 = analyzer.next_lexeme();
	REQUIRE( lex4.type() == cfgfile::lexeme_type_t::finish );
}

TEST_CASE( "test_startTag" )
{
	std::stringstream stream( "{cfg a{}" );

	cfgfile::input_stream_t<> input( "test_backSlash", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	auto lex1 = analyzer.next_lexeme();
	REQUIRE( lex1.type() == cfgfile::lexeme_type_t::start );

	auto lex2 = analyzer.next_lexeme();
	REQUIRE( lex2.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex2.value() == "cfg" );

	auto lex3 = analyzer.next_lexeme();
	REQUIRE( lex3.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex3.value() == "a" );

	auto lex4 = analyzer.next_lexeme();
	REQUIRE( lex4.type() == cfgfile::lexeme_type_t::start );

	auto lex5 = analyzer.next_lexeme();
	REQUIRE( lex5.type() == cfgfile::lexeme_type_t::finish );
}

TEST_CASE( "test_vertBar" )
{
	std::stringstream stream( "{cfg |a" );

	cfgfile::input_stream_t<> input( "test_backSlash", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	auto lex1 = analyzer.next_lexeme();
	REQUIRE( lex1.type() == cfgfile::lexeme_type_t::start );

	auto lex2 = analyzer.next_lexeme();
	REQUIRE( lex2.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex2.value() == "cfg" );

	auto lex3 = analyzer.next_lexeme();
	REQUIRE( lex3.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex3.value() == "|a" );
}

TEST_CASE( "test_vertBar2" )
{
	std::stringstream stream( "{cfg |" );

	cfgfile::input_stream_t<> input( "test_backSlash", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	auto lex1 = analyzer.next_lexeme();
	REQUIRE( lex1.type() == cfgfile::lexeme_type_t::start );

	auto lex2 = analyzer.next_lexeme();
	REQUIRE( lex2.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex2.value() == "cfg" );

	auto lex3 = analyzer.next_lexeme();
	REQUIRE( lex3.type() == cfgfile::lexeme_type_t::string );
	REQUIRE( lex3.value() == "|" );

	auto lex4 = analyzer.next_lexeme();

	REQUIRE( lex4.type() == cfgfile::lexeme_type_t::null );
}
