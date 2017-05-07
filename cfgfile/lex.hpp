
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

#ifndef CFGFILE__LEX_HPP__INCLUDED
#define CFGFILE__LEX_HPP__INCLUDED

// cfgfile include.
#include "types.hpp"
#include "input_stream.hpp"
#include "exceptions.hpp"


namespace cfgfile {

//
// lexeme_type_t
//

//! Type of the lexeme.
enum class lexeme_type_t {
    //! Null lexeme.
    null,
    //! Start tag lexeme "{".
    start,
    //! Finish tag lexeme "}".
    finish,
    //! String lexeme.
    string
}; // enum class lexeme_type_t


//
// lexeme_t
//

//! Lexeme.
class lexeme_t final {
public:
	lexeme_t()
		:	m_type( lexeme_type_t::null )
	{
	}

    lexeme_t( lexeme_type_t type, const string_t & value )
		:	m_type( type )
		,	m_value( value )
	{
	}

    //! \return Lexeme type.
    lexeme_type_t type() const
	{
		return m_type;
	}

    //! \return Lexeme value.
    const string_t & value() const
	{
		return m_value;
	}

    //! \return Is lexeme a null lexeme.
    bool is_null() const
	{
		return ( m_type == lexeme_type_t::null );
	}

private:
    //! Lexeme type.
    lexeme_type_t m_type;
    //! Lexeme value.
    string_t m_value;
}; // class lexeme_t


//
// lexical_analyzer_t
//

//! Lexical analyzer.
class lexical_analyzer_t final {
public:
	explicit lexical_analyzer_t( input_stream_t & stream )
		:	m_stream( stream )
		,	m_line_number( m_stream.line_number() )
		,	m_column_number( m_stream.column_number() )
	{
	}

	/*!
		\return Next lexeme.

		\throw Exception on lexical error.
	*/
	lexeme_t next_lexeme()
	{
		string_t result;

		bool quoted_lexeme = false;
		bool first_symbol = true;
		bool skip_comment = false;

		skip_spaces();

		m_line_number = m_stream.line_number();
		m_column_number = m_stream.column_number();

		if( m_stream.at_end() )
			return lexeme_t( lexeme_type_t::null, string_t() );

		while( true )
		{
			char_t ch = m_stream.get();

			if( ch == c_quotes )
			{
				if( quoted_lexeme )
					break;
				else if( first_symbol )
					quoted_lexeme = true;
				else
				{
					m_stream.put_back( ch );

					break;
				}
			}
			else if( ch == c_back_slash )
			{
				char_t new_char = 0x00;

				if( !quoted_lexeme )
					result.push_back( ch );
				else if( process_back_slash( new_char ) )
					result.push_back( new_char );
				else
					throw exception_t( string_t(
						SL( "Unrecognized back-slash sequence: \"\\" ) ) +
						SL( "\". In file \"" ) + m_stream.file_name() +
						SL( "\" on line " ) + std::to_string( line_number() ) +
						SL( "." ) );
			}
			else if( ch == c_begin_tag )
			{
				if( result.empty() )
					return lexeme_t( lexeme_type_t::start, string_t( 1, ch ) );
				else if( quoted_lexeme )
					result.push_back( ch );
				else
				{
					m_stream.put_back( ch );

					break;
				}
			}
			else if( ch == c_end_tag )
			{
				if( result.empty() )
					return lexeme_t( lexeme_type_t::finish, string_t( 1, ch ) );
				else if( quoted_lexeme )
					result.push_back( ch );
				else
				{
					m_stream.put_back( ch );

					break;
				}
			}
			else if( ch == c_space || ch == c_tab )
			{
				if( quoted_lexeme )
					result.push_back( ch );
				else
					break;
			}
			else if( ch == c_carriage_return || ch == c_line_feed )
			{
				if( quoted_lexeme )
					throw exception_t( string_t( SL( "Unfinished quoted lexeme. " ) ) +
						SL( "New line detected. In file \"" ) +
						m_stream.file_name() +
						SL( "\" on line " ) + std::to_string( line_number() ) +
						SL( "." ) );
				else
					break;
			}
			else if( ch == c_vertical_bar )
			{
				if( quoted_lexeme )
					result.push_back( ch );
				else
				{
					char_t next_char = m_stream.get();

					if( next_char == c_vertical_bar )
					{
						skip_comment = true;

						skip_one_line_comment();

						if( first_symbol )
							skip_spaces();
						else
							break;
					}
					else if( next_char == c_sharp )
					{
						skip_comment = true;

						skip_multi_line_comment();

						if( first_symbol )
							skip_spaces();
						else
							break;
					}
					else
					{
						result.push_back( ch );

						m_stream.put_back( next_char );
					}
				}
			}
			else
				result.push_back( ch );

			if( m_stream.at_end() )
			{
				if( quoted_lexeme )
					throw exception_t( string_t( SL( "Unfinished quoted lexeme. " ) ) +
						SL( "End of file riched. In file \"" ) +
						m_stream.file_name() +
						SL( "\" on line " ) + std::to_string( line_number() ) +
						SL( "." ) );
				else if( result.empty() )
					return lexeme_t( lexeme_type_t::null, string_t() );
				else
					break;
			}

			if( !skip_comment )
				first_symbol = false;
			else
				skip_comment = false;
		}

		return lexeme_t( lexeme_type_t::string, result );
	}

    //! \return Input stream.
    input_stream_t & input_stream()
	{
		return m_stream;
	}

	//! \return Line number.
	pos_t line_number() const
	{
		return m_line_number;
	}

	//! \return Column number.
	pos_t column_number() const
	{
		return m_column_number;
	}

private:
	//! \return Is character a space character?
	bool is_space_char( char_t ch )
	{
		if( ch == c_space || ch == c_tab ||
			ch == c_carriage_return || ch == c_line_feed )
				return true;
		else
			return false;
	}

	//! Skip spaces in the stream.
	void skip_spaces()
	{
		if( m_stream.at_end() )
			return;

		char_t ch = m_stream.get();

		while( is_space_char( ch ) )
		{
			if( m_stream.at_end() )
				return;

			ch = m_stream.get();
		}

		m_stream.put_back( ch );
	}

	//! Process back-slash sequence.
	bool process_back_slash( char_t & ch )
	{
		if( m_stream.at_end() )
			throw exception_t( string_t( SL( "Unexpected end of file. "
					"Unfinished back slash sequence. In file \"" ) ) +
				m_stream.file_name() + SL( "\" on line " ) +
				std::to_string( m_stream.line_number() ) + SL( "." ) );

		ch = m_stream.get();

		if( ch == c_n )
			ch = c_carriage_return;
		else if( ch == c_t )
			ch = c_tab;
		else if( ch == c_r )
			ch = c_line_feed;
		else if( ch == c_quotes )
			ch = c_quotes;
		else if( ch == c_back_slash )
			ch = c_back_slash;
		else
			return false;

		return true;
	}

	//! Skip one-line comment.
	void skip_one_line_comment()
	{
		if( !m_stream.at_end() )
		{
			char_t ch = m_stream.get();

			while( ch != c_carriage_return && ch != c_line_feed &&
				!m_stream.at_end() )
					ch = m_stream.get();
		}
	}

	//! Skip multi-line comment.
	void skip_multi_line_comment()
	{
		if( !m_stream.at_end() )
		{
			char_t ch = m_stream.get();

			if( m_stream.at_end() )
				return;

			char_t next_char = m_stream.get();

			if( ch == c_sharp && next_char == c_vertical_bar )
				return;

			while( !m_stream.at_end() )
			{
				ch = next_char;

				next_char = m_stream.get();

				if( ch == c_sharp && next_char == c_vertical_bar )
					break;
			}
		}
	}

private:
	DISABLE_COPY( lexical_analyzer_t )

	//! Input stream.
	input_stream_t & m_stream;
	//! Line number.
	pos_t m_line_number;
	//! Column number.
	pos_t m_column_number;
}; // class lexical_analyzer_t

} /* namespace cfgfile */

#endif // CFGFILE__LEX_HPP__INCLUDED
