
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

// UnitTest include.
#include <UnitTest/unit_test.hpp>

// cfgfile include.
#include <cfgfile/lex.hpp>
#include <cfgfile/input_stream.hpp>

// C++ include.
#include <sstream>


TEST( LexicalAnalyzer, test_quotedLexeme1 )
{
	std::stringstream stream( "{firstTag \"lexeme\"}" );

	cfgfile::input_stream_t<> input( "test_quotedLexeme1", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	cfgfile::lexeme_t<> lex1 = analyzer.next_lexeme();
	CHECK_CONDITION( lex1.type() == cfgfile::lexeme_type_t::start );

	cfgfile::lexeme_t<> lex2 = analyzer.next_lexeme();
	CHECK_CONDITION( lex2.type() == cfgfile::lexeme_type_t::string );
	CHECK_CONDITION( lex2.value() == "firstTag" );

	cfgfile::lexeme_t<> lex3 = analyzer.next_lexeme();
	CHECK_CONDITION( lex3.type() == cfgfile::lexeme_type_t::string );
	CHECK_CONDITION( lex3.value() == "lexeme" );

	cfgfile::lexeme_t<> lex4 = analyzer.next_lexeme();
	CHECK_CONDITION( lex4.type() == cfgfile::lexeme_type_t::finish );

	CHECK_CONDITION( input.at_end() == true );
}

TEST( LexicalAnalyzer, test_quotedLexeme2 )
{
	std::stringstream stream( " \r\n\t {firstTag\"lexeme\"}" );

	cfgfile::input_stream_t<> input( "test_quotedLexeme2", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	cfgfile::lexeme_t<> lex1 = analyzer.next_lexeme();
	CHECK_CONDITION( lex1.type() == cfgfile::lexeme_type_t::start );

	cfgfile::lexeme_t<> lex2 = analyzer.next_lexeme();
	CHECK_CONDITION( lex2.type() == cfgfile::lexeme_type_t::string );
	CHECK_CONDITION( lex2.value() == "firstTag" );

	cfgfile::lexeme_t<> lex3 = analyzer.next_lexeme();
	CHECK_CONDITION( lex3.type() == cfgfile::lexeme_type_t::string );
	CHECK_CONDITION( lex3.value() == "lexeme" );

	cfgfile::lexeme_t<> lex4 = analyzer.next_lexeme();
	CHECK_CONDITION( lex4.type() == cfgfile::lexeme_type_t::finish );

	CHECK_CONDITION( input.at_end() == true );
}

TEST( LexicalAnalyzer, test_backSlahSequence )
{
	std::stringstream stream( " \r\n\t {firstTag\"\\n\\r\\t\\\"\\\\lexeme\"}" );

	cfgfile::input_stream_t<> input( "test_backSlahSequence", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	cfgfile::lexeme_t<> lex1 = analyzer.next_lexeme();
	CHECK_CONDITION( lex1.type() == cfgfile::lexeme_type_t::start );

	cfgfile::lexeme_t<> lex2 = analyzer.next_lexeme();
	CHECK_CONDITION( lex2.type() == cfgfile::lexeme_type_t::string );
	CHECK_CONDITION( lex2.value() == "firstTag" );

	cfgfile::lexeme_t<> lex3 = analyzer.next_lexeme();
	CHECK_CONDITION( lex3.type() == cfgfile::lexeme_type_t::string );
	CHECK_CONDITION( lex3.value() == "\n\r\t\"\\lexeme" );

	cfgfile::lexeme_t<> lex4 = analyzer.next_lexeme();
	CHECK_CONDITION( lex4.type() == cfgfile::lexeme_type_t::finish );

	CHECK_CONDITION( input.at_end() == true );
}

TEST( LexicalAnalyzer, test_wrongBackSlahSequence )
{
	std::stringstream stream( " \r\n\t {firstTag\"\\glexeme\"}" );

	cfgfile::input_stream_t<> input( "test_wrongBackSlahSequence", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	cfgfile::lexeme_t<> lex1 = analyzer.next_lexeme();
	CHECK_CONDITION( lex1.type() == cfgfile::lexeme_type_t::start );

	cfgfile::lexeme_t<> lex2 = analyzer.next_lexeme();
	CHECK_CONDITION( lex2.type() == cfgfile::lexeme_type_t::string );
	CHECK_CONDITION( lex2.value() == "firstTag" );

	try {
		cfgfile::lexeme_t<> lex3 = analyzer.next_lexeme();

		CHECK_CONDITION( true == false );
	}
	catch( const cfgfile::exception_t<> & x )
	{
		CHECK_CONDITION( x.desc() == "Unrecognized back-slash sequence: \"\\g\". "
			"In file \"test_wrongBackSlahSequence\" on line 2." );
	}
}

TEST( LexicalAnalyzer, test_oneLineComment )
{
	std::stringstream stream( "{firstTag \"value1\"}\r\n"
		"|| Comment\r\n"
		"{secondTag \"value2\"}\r\n" );

	cfgfile::input_stream_t<> input( "test_oneLineComment", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	cfgfile::lexeme_t<> lex1 = analyzer.next_lexeme();
	CHECK_CONDITION( lex1.type() == cfgfile::lexeme_type_t::start );

	cfgfile::lexeme_t<> lex2 = analyzer.next_lexeme();
	CHECK_CONDITION( lex2.type() == cfgfile::lexeme_type_t::string );
	CHECK_CONDITION( lex2.value() == "firstTag" );

	cfgfile::lexeme_t<> lex3 = analyzer.next_lexeme();
	CHECK_CONDITION( lex3.type() == cfgfile::lexeme_type_t::string );
	CHECK_CONDITION( lex3.value() == "value1" );

	cfgfile::lexeme_t<> lex4 = analyzer.next_lexeme();
	CHECK_CONDITION( lex4.type() == cfgfile::lexeme_type_t::finish );

	cfgfile::lexeme_t<> lex5 = analyzer.next_lexeme();
	CHECK_CONDITION( lex5.type() == cfgfile::lexeme_type_t::start );

	cfgfile::lexeme_t<> lex6 = analyzer.next_lexeme();
	CHECK_CONDITION( lex6.type() == cfgfile::lexeme_type_t::string );
	CHECK_CONDITION( lex6.value() == "secondTag" );

	cfgfile::lexeme_t<> lex7 = analyzer.next_lexeme();
	CHECK_CONDITION( lex7.type() == cfgfile::lexeme_type_t::string );
	CHECK_CONDITION( lex7.value() == "value2" );

	cfgfile::lexeme_t<> lex8 = analyzer.next_lexeme();
	CHECK_CONDITION( lex8.type() == cfgfile::lexeme_type_t::finish );
}

TEST( LexicalAnalyzer, test_multiLineComment )
{
	std::stringstream stream( "{firstTag \"value1\"}\r\n"
		"|#\r\n"
		"  Comment\r\n"
		"#|\r\n"
		"{secondTag \"value2\"}\r\n" );

	cfgfile::input_stream_t<> input( "test_multiLineComment", stream );
	cfgfile::lexical_analyzer_t<> analyzer( input );

	cfgfile::lexeme_t<> lex1 = analyzer.next_lexeme();
	CHECK_CONDITION( lex1.type() == cfgfile::lexeme_type_t::start );

	cfgfile::lexeme_t<> lex2 = analyzer.next_lexeme();
	CHECK_CONDITION( lex2.type() == cfgfile::lexeme_type_t::string );
	CHECK_CONDITION( lex2.value() == "firstTag" );

	cfgfile::lexeme_t<> lex3 = analyzer.next_lexeme();
	CHECK_CONDITION( lex3.type() == cfgfile::lexeme_type_t::string );
	CHECK_CONDITION( lex3.value() == "value1" );

	cfgfile::lexeme_t<> lex4 = analyzer.next_lexeme();
	CHECK_CONDITION( lex4.type() == cfgfile::lexeme_type_t::finish );

	cfgfile::lexeme_t<> lex5 = analyzer.next_lexeme();
	CHECK_CONDITION( lex5.type() == cfgfile::lexeme_type_t::start );

	cfgfile::lexeme_t<> lex6 = analyzer.next_lexeme();
	CHECK_CONDITION( lex6.type() == cfgfile::lexeme_type_t::string );
	CHECK_CONDITION( lex6.value() == "secondTag" );

	cfgfile::lexeme_t<> lex7 = analyzer.next_lexeme();
	CHECK_CONDITION( lex7.type() == cfgfile::lexeme_type_t::string );
	CHECK_CONDITION( lex7.value() == "value2" );

	cfgfile::lexeme_t<> lex8 = analyzer.next_lexeme();
	CHECK_CONDITION( lex8.type() == cfgfile::lexeme_type_t::finish );
}

int main()
{
	RUN_ALL_TESTS()

	return 0;
}
