
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
#include <cfgfile/all.hpp>

// C++ include.
#include <sstream>


class ThirdTag
	:	public cfgfile::tag_t<>
{
public:
	explicit ThirdTag( tag_t<> & owner )
		:	cfgfile::tag_t<>( owner, "thirdTag", false )
		,	m_started( false )
		,	m_finished( false )
		,	m_withString( false )
	{
	}

	bool isStarted() const
	{
		return m_started;
	}

	bool isFinished() const
	{
		return m_finished;
	}

	bool isWithString() const
	{
		return m_withString;
	}

	std::string print( int indent = 0 ) const override
	{
		(void) indent;
		return std::string();
	}

protected:
	void on_start( const cfgfile::parser_info_t<> & ) override
	{
		m_started = true;
	}

	void on_finish( const cfgfile::parser_info_t<> & ) override
	{
		m_finished = true;
	}

	void on_string( const cfgfile::parser_info_t<> &,
		const std::string & ) override
	{
		m_withString = true;
	}

private:
	bool m_started;
	bool m_finished;
	bool m_withString;
}; // class ThirdTag


class SecondTag
	:	public cfgfile::tag_t<>
{
public:
	explicit SecondTag( tag_t<> & owner )
		:	cfgfile::tag_t<>( owner, "secondTag", false )
		,	m_started( false )
		,	m_finished( false )
		,	m_withString( false )
		,	m_child( *this )
	{
	}

	const ThirdTag & thirdTag() const
	{
		return m_child;
	}

	bool isStarted() const
	{
		return m_started;
	}

	bool isFinished() const
	{
		return m_finished;
	}

	bool isWithString() const
	{
		return m_withString;
	}

	std::string print( int indent = 0 ) const override
	{
		(void) indent;
		return std::string();
	}

protected:
	void on_start( const cfgfile::parser_info_t<> & ) override
	{
		m_started = true;
	}

	void on_finish( const cfgfile::parser_info_t<> & ) override
	{
		m_finished = true;
	}

	void on_string( const cfgfile::parser_info_t<> &,
		const std::string & ) override
	{
		m_withString = true;
	}

private:
	bool m_started;
	bool m_finished;
	bool m_withString;
	ThirdTag m_child;
}; // class SecondTag


class FirstTag
	:	public cfgfile::tag_t<>
{
public:
	FirstTag()
		:	cfgfile::tag_t<>( "firstTag", false )
		,	m_started( false )
		,	m_finished( false )
		,	m_withString( false )
		,	m_child( *this )
	{
	}

	const SecondTag & secondTag() const
	{
		return m_child;
	}

	bool isStarted() const
	{
		return m_started;
	}

	bool isFinished() const
	{
		return m_finished;
	}

	bool isWithString() const
	{
		return m_withString;
	}

	std::string print( int indent = 0 ) const override
	{
		(void) indent;
		return std::string();
	}

protected:
	void on_start( const cfgfile::parser_info_t<> & ) override
	{
		m_started = true;
	}

	void on_finish( const cfgfile::parser_info_t<> & ) override
	{
		m_finished = true;
	}

	void on_string( const cfgfile::parser_info_t<> &,
		const std::string & ) override
	{
		m_withString = true;
	}

private:
	bool m_started;
	bool m_finished;
	bool m_withString;
	SecondTag m_child;
}; // class FirstTag

TEST( Parser, test_configWithOneTag )
{
	std::stringstream stream( "{firstTag \"lexeme1\"}" );

	cfgfile::input_stream_t<> input( "test_configWithOneTag", stream );

	FirstTag firstTag;

	cfgfile::parser_t<> parser( firstTag, input );

	parser.parse( "test_configWithOneTag" );

	firstTag.set_defined();

	CHECK_CONDITION( firstTag.is_defined() == true );
	CHECK_CONDITION( firstTag.isStarted() == true );
	CHECK_CONDITION( firstTag.isFinished() == true );
	CHECK_CONDITION( firstTag.isWithString() == true );

	CHECK_CONDITION( firstTag.secondTag().is_defined() == false );
	CHECK_CONDITION( firstTag.secondTag().isStarted() == false );
	CHECK_CONDITION( firstTag.secondTag().isFinished() == false );
	CHECK_CONDITION( firstTag.secondTag().isWithString() == false );

	CHECK_CONDITION( firstTag.secondTag().thirdTag().is_defined() == false );
	CHECK_CONDITION( firstTag.secondTag().thirdTag().isStarted() == false );
	CHECK_CONDITION( firstTag.secondTag().thirdTag().isFinished() == false );
	CHECK_CONDITION( firstTag.secondTag().thirdTag().isWithString() == false );
} // test_configWithOneTag

TEST( Parser, test_configWithThreeTags )
{
	std::stringstream stream( "{firstTag \"lexeme1\"\r\n"
		"\t{secondTag \"lexeme2\"\r\n"
		"\t\t{thirdTag \"lexeme3\"}\r\n"
		"}\r\n"
		"}" );

	cfgfile::input_stream_t<> input( "test_configWithThreeTags", stream );

	FirstTag firstTag;

	cfgfile::parser_t<> parser( firstTag, input );

	parser.parse( "test_configWithThreeTags" );

	firstTag.set_defined();

	CHECK_CONDITION( firstTag.is_defined() == true );
	CHECK_CONDITION( firstTag.isStarted() == true );
	CHECK_CONDITION( firstTag.isFinished() == true );
	CHECK_CONDITION( firstTag.isWithString() == true );

	CHECK_CONDITION( firstTag.secondTag().is_defined() == false );
	CHECK_CONDITION( firstTag.secondTag().isStarted() == true );
	CHECK_CONDITION( firstTag.secondTag().isFinished() == true );
	CHECK_CONDITION( firstTag.secondTag().isWithString() == true );

	CHECK_CONDITION( firstTag.secondTag().thirdTag().is_defined() == false );
	CHECK_CONDITION( firstTag.secondTag().thirdTag().isStarted() == true );
	CHECK_CONDITION( firstTag.secondTag().thirdTag().isFinished() == true );
	CHECK_CONDITION( firstTag.secondTag().thirdTag().isWithString() == true );
} // test_configWithThreeTags

TEST( Parser, test_unexpectedStartCurlBrace )
{
	std::stringstream stream( "{{firstTag \"lexeme1\"}" );

	cfgfile::input_stream_t<> input( "test_unexpectedStartCurlBrace", stream );

	FirstTag firstTag;

	cfgfile::parser_t<> parser( firstTag, input );

	try {
		parser.parse( "test_unexpectedStartCurlBrace" );
	}
	catch( cfgfile::exception_t<> & x )
	{
		CHECK_CONDITION( x.desc() ==
			"Unexpected start curl brace. "
			"We expected tag name, but we've got start curl brace. "
			"In file \"test_unexpectedStartCurlBrace\" on line 1." );

		return;
	}

	CHECK_CONDITION( false );
} // test_unexpectedStartCurlBrace

TEST( Parser, test_unexpectedTagName1 )
{
	std::stringstream stream( "{secondTag \"lexeme1\"}" );

	cfgfile::input_stream_t<> input( "test_unexpectedTagName1", stream );

	FirstTag firstTag;

	cfgfile::parser_t<> parser( firstTag, input );

	try {
		parser.parse( "test_unexpectedTagName1" );
	}
	catch( cfgfile::exception_t<> & x )
	{
		CHECK_CONDITION( x.desc() ==
			"Unexpected tag name. "
			"We expected \"firstTag\", but we've got \"secondTag\". "
			"In file \"test_unexpectedTagName1\" on line 1." );

		return;
	}

	CHECK_CONDITION( false );
} // test_unexpectedTagName1

TEST( Parser, test_unexpectedTagName2 )
{
	std::stringstream stream( "{firstTag \"lexeme1\"\r\n"
		"\t{thirdTag \"lexeme2\"}\r\n"
		"}" );

	cfgfile::input_stream_t<> input( "test_unexpectedTagName2", stream );

	FirstTag firstTag;

	cfgfile::parser_t<> parser( firstTag, input );

	try {
		parser.parse( "test_unexpectedTagName2" );
	}
	catch( cfgfile::exception_t<> & x )
	{
		CHECK_CONDITION( x.desc() ==
			"Unexpected tag name. "
			"We expected one child tag of tag \"firstTag\", "
			"but we've got \"thirdTag\". "
			"In file \"test_unexpectedTagName2\" on line 2." );

		return;
	}

	CHECK_CONDITION( false );
} // test_unexpectedTagName2

TEST( Parser, test_unexpectedEndOfFile )
{
	std::stringstream stream( "{firstTag \"lexeme1\"\r\n"
		"\t{secondTag \"lexeme2\"}\r\n" );

	cfgfile::input_stream_t<> input( "test_unexpectedEndOfFile", stream );

	FirstTag firstTag;

	cfgfile::parser_t<> parser( firstTag, input );

	try {
		parser.parse( "test_unexpectedEndOfFile" );
	}
	catch( cfgfile::exception_t<> & x )
	{
		CHECK_CONDITION( x.desc() ==
			"Unexpected end of file. "
			"Still unfinished tag \"firstTag\"." );

		return;
	}

	CHECK_CONDITION( false );
} // test_unexpectedEndOfFile

class empty_tag_t
	:	public cfgfile::tag_t<>
{
public:
	empty_tag_t( const std::string & name, bool is_mandatory = false )
		:	cfgfile::tag_t<> ( name, is_mandatory )
	{
	}

	virtual ~empty_tag_t()
	{
	}

	std::string print( int indent = 0 ) const override
	{
		(void) indent;
	}

	void on_finish( const cfgfile::parser_info_t<> & ) override
	{
	}

	void on_string( const cfgfile::parser_info_t<> &,
		const typename std::string & ) override
	{
	}
};

TEST( Parser, test_undefinedFirstMandatoryTag )
{
	std::stringstream stream( "{cfg}" );

	cfgfile::input_stream_t<> input( "test_undefinedFirstMandatoryTag", stream );

	empty_tag_t tag( "cfg", true );

	cfgfile::parser_t<> parser( tag, input );

	try {
		parser.parse( "test_undefinedFirstMandatoryTag" );

		CHECK_CONDITION( false );
	}
	catch( cfgfile::exception_t<> & x )
	{
		CHECK_CONDITION( x.desc() ==
			"Undefined mandatory tag: \"cfg\"." );
	}
} // test_unexpectedEndOfFile


int main()
{
	RUN_ALL_TESTS()

	return 0;
}
