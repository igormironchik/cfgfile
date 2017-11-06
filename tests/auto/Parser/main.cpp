
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

	empty_tag_t( cfgfile::tag_t<> & parent,
		const std::string & name, bool is_mandatory = false )
		:	cfgfile::tag_t<> ( parent, name, is_mandatory )
	{
	}

	virtual ~empty_tag_t()
	{
	}

	std::string print( int indent = 0 ) const override
	{
		(void) indent;

		return std::string();
	}

	void on_finish( const cfgfile::parser_info_t<> & ) override
	{
	}

	void on_string( const cfgfile::parser_info_t<> &,
		const typename std::string & ) override
	{
	}
};

TEST( Parser, test_undefined_first_mandatory_tag )
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
}

TEST( Parser, test_undefined_first_tag )
{
	std::stringstream stream( "" );

	cfgfile::input_stream_t<> input( "test_undefinedFirstTag", stream );

	empty_tag_t tag( "cfg" );

	cfgfile::parser_t<> parser( tag, input );

	parser.parse( "test_undefinedFirstTag" );

	CHECK_CONDITION( true );
}

TEST( Parser, test_unexpected_content )
{
	std::stringstream stream( "{cfg} abc" );

	cfgfile::input_stream_t<> input( "test_unexpectedContent", stream );

	empty_tag_t tag( "cfg" );

	cfgfile::parser_t<> parser( tag, input );

	try {
		parser.parse( "test_unexpectedContent" );

		CHECK_CONDITION( false );
	}
	catch( cfgfile::exception_t<> & x )
	{
		CHECK_CONDITION( x.desc() == "Unexpected content. "
			"We've finished parsing, but we've got this: \"abc\". "
			"In file \"test_unexpectedContent\" on line 1." )
	}
}

TEST( Parser, test_unexpected_content_2 )
{
	std::stringstream stream( "abc" );

	cfgfile::input_stream_t<> input( "test_unexpectedContent", stream );

	empty_tag_t tag( "cfg" );

	cfgfile::parser_t<> parser( tag, input );

	try {
		parser.parse( "test_unexpectedContent" );

		CHECK_CONDITION( false );
	}
	catch( cfgfile::exception_t<> & x )
	{
		CHECK_CONDITION( x.desc() == "Expected start curl brace, "
			"but we've got \"abc\". In file \"test_unexpectedContent\" "
			"on line 1." )
	}
}

TEST( Parser, test_unexpected_finish )
{
	std::stringstream stream( "{}" );

	cfgfile::input_stream_t<> input( "test_unexpectedFinish", stream );

	empty_tag_t tag( "cfg" );

	cfgfile::parser_t<> parser( tag, input );

	try {
		parser.parse( "test_unexpectedFinish" );

		CHECK_CONDITION( false );
	}
	catch( cfgfile::exception_t<> & x )
	{
		CHECK_CONDITION( x.desc() == "Unexpected finish curl brace. "
			"We expected tag name, but we've got finish curl brace. "
			"In file \"test_unexpectedFinish\" on line 1." )
	}
}

TEST( Parser, test_unexpected_end_of_file_2 )
{
	std::stringstream stream( "{" );

	cfgfile::input_stream_t<> input( "test_unexpectedEndOfFile", stream );

	empty_tag_t tag( "cfg" );

	cfgfile::parser_t<> parser( tag, input );

	try {
		parser.parse( "test_unexpectedEndOfFile" );

		CHECK_CONDITION( false );
	}
	catch( cfgfile::exception_t<> & x )
	{
		CHECK_CONDITION( x.desc() == "Unexpected end of file. "
			"In file \"test_unexpectedEndOfFile\" on line 1." )
	}
}

TEST( Parser, test_tag_no_value_with_value )
{
	std::stringstream stream( "{cfg abc}" );

	cfgfile::input_stream_t<> input( "test_tagNoValueWithValue", stream );

	cfgfile::tag_no_value_t<> tag( "cfg" );

	cfgfile::parser_t<> parser( tag, input );

	try {
		parser.parse( "test_tagNoValueWithValue" );

		CHECK_CONDITION( false );
	}
	catch( cfgfile::exception_t<> & x )
	{
		CHECK_CONDITION( x.desc() == "Tag \"cfg\" doesn't allow any values. "
			"But we've got this: \"abc\". In file \"test_tagNoValueWithValue\" "
			"on line 1." )
	}
}

TEST( Parser, test_tag_redefinition )
{
	try {
		cfgfile::tag_no_value_t<> tag( "cfg" );
		cfgfile::tag_no_value_t<> c1( tag, "c" );
		cfgfile::tag_no_value_t<> c2( tag, "c" );

		CHECK_CONDITION( false );
	}
	catch( cfgfile::exception_t<> & x )
	{
		CHECK_CONDITION( x.desc() == "Tag with name \"c\" "
			"already exists in parent tag \"cfg\"." )
	}
}

TEST( Parser, test_tag_scalar_vector_set_value )
{
	cfgfile::tag_scalar_vector_t< int > tag( "cfg" );
	cfgfile::constraint_one_of_t< int > c;
	c.add_value( 100 );
	c.add_value( 200 );
	c.add_value( 300 );
	tag.set_constraint( &c );


	tag.set_value( 100 );
	tag.set_value( 200 );
	tag.set_value( 300 );

	CHECK_CONDITION( tag.values().size() == 3 )
	CHECK_CONDITION( tag.values().at( 0 ) == 100 )
	CHECK_CONDITION( tag.values().at( 1 ) == 200 )
	CHECK_CONDITION( tag.values().at( 2 ) == 300 )

	try {
		tag.set_value( 400 );

		CHECK_CONDITION( false )
	}
	catch( const cfgfile::exception_t<> & x )
	{
		CHECK_CONDITION( x.desc() == "Invalid value: \"400\". "
			"Value must match to the constraint in tag \"cfg\"." )
	}
}

TEST( Parser, test_tag_scalar_vector_undefined_child )
{
	std::stringstream stream( "{cfg}" );

	cfgfile::input_stream_t<> input( "test_tag_scalar_vector_undefined_child", stream );

	cfgfile::tag_scalar_vector_t< int > tag( "cfg", true );
	cfgfile::tag_no_value_t<> c( tag, "child", true );

	cfgfile::parser_t<> parser( tag, input );

	try {
		parser.parse( "test_tag_scalar_vector_undefined_child" );

		CHECK_CONDITION( false );
	}
	catch( cfgfile::exception_t<> & x )
	{
		CHECK_CONDITION( x.desc() == "Undefined child mandatory tag: \"child\". "
			"Where parent is: \"cfg\". In file \"test_tag_scalar_vector_undefined_child\" "
			"on line 1." )
	}
}

TEST( Parser, test_tag_scalar_vector_wrong_place_for_value )
{
	std::stringstream stream( "{cfg 100 {child} 200}" );

	cfgfile::input_stream_t<> input( "test_tag_scalar_vector_wrong_place_for_value", stream );

	cfgfile::tag_scalar_vector_t< int > tag( "cfg", true );
	cfgfile::tag_no_value_t<> c( tag, "child", true );

	cfgfile::parser_t<> parser( tag, input );

	try {
		parser.parse( "test_tag_scalar_vector_wrong_place_for_value" );

		CHECK_CONDITION( false );
	}
	catch( cfgfile::exception_t<> & x )
	{
		CHECK_CONDITION( x.desc() == "Value \"200\" for tag \"cfg\" "
			"must be defined before any child tag. In file "
			"\"test_tag_scalar_vector_wrong_place_for_value\" on line 1." )
	}
}


int main()
{
	RUN_ALL_TESTS()

	return 0;
}
