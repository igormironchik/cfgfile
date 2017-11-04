
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
template< typename Trait = string_trait_t >
class lexeme_t final {
public:
	lexeme_t()
		:	m_type( lexeme_type_t::null )
	{
	}

    lexeme_t( lexeme_type_t type, const typename Trait::string_t & value )
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
    const typename Trait::string_t & value() const
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
    typename Trait::string_t m_value;
}; // class lexeme_t


//
// lexical_analyzer_t
//

//! Lexical analyzer.
template< typename Trait = string_trait_t >
class lexical_analyzer_t final {
public:
	explicit lexical_analyzer_t( input_stream_t< Trait > & stream )
		:	m_stream( stream )
		,	m_line_number( m_stream.line_number() )
		,	m_column_number( m_stream.column_number() )
	{
	}

	/*!
		\return Next lexeme.

		\throw Exception on lexical error.
	*/
	lexeme_t< Trait > next_lexeme()
	{
		typename Trait::string_t result;

		bool quoted_lexeme = false;
		bool first_symbol = true;
		bool skip_comment = false;

		skip_spaces();

		m_line_number = m_stream.line_number();
		m_column_number = m_stream.column_number();

		if( m_stream.at_end() )
			return lexeme_t< Trait >( lexeme_type_t::null,
				typename Trait::string_t() );

		while( true )
		{
			typename Trait::char_t ch = m_stream.get();

			if( ch == const_t< Trait >::c_quotes )
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
			else if( ch == const_t< Trait >::c_back_slash )
			{
				typename Trait::char_t new_char = 0x00;

				if( !quoted_lexeme )
					result.push_back( ch );
				else if( process_back_slash( new_char ) )
					result.push_back( new_char );
				else
					throw exception_t< Trait >(
						Trait::from_ascii( "Unrecognized back-slash "
							"sequence: \"\\" ) +
						typename Trait::string_t( 1, new_char ) +
						Trait::from_ascii( "\". In file \"" ) +
						m_stream.file_name() +
						Trait::from_ascii( "\" on line " ) +
						Trait::to_string( line_number() ) +
						Trait::from_ascii( "." ) );
			}
			else if( ch == const_t< Trait >::c_begin_tag )
			{
				if( result.empty() )
					return lexeme_t< Trait >( lexeme_type_t::start,
						typename Trait::string_t( 1, ch ) );
				else if( quoted_lexeme )
					result.push_back( ch );
				else
				{
					m_stream.put_back( ch );

					break;
				}
			}
			else if( ch == const_t< Trait >::c_end_tag )
			{
				if( result.empty() )
					return lexeme_t< Trait >( lexeme_type_t::finish,
						typename Trait::string_t( 1, ch ) );
				else if( quoted_lexeme )
					result.push_back( ch );
				else
				{
					m_stream.put_back( ch );

					break;
				}
			}
			else if( ch == const_t< Trait >::c_space ||
				ch == const_t< Trait >::c_tab )
			{
				if( quoted_lexeme )
					result.push_back( ch );
				else
					break;
			}
			else if( ch == const_t< Trait >::c_carriage_return ||
				ch == const_t< Trait >::c_line_feed )
			{
				if( quoted_lexeme )
					throw exception_t< Trait >(
						Trait::from_ascii( "Unfinished quoted lexeme. " ) +
						Trait::from_ascii( "New line detected. In file \"" ) +
						m_stream.file_name() +
						Trait::from_ascii( "\" on line " ) +
						Trait::to_string( line_number() ) +
						Trait::from_ascii( "." ) );
				else
					break;
			}
			else if( ch == const_t< Trait >::c_vertical_bar )
			{
				if( quoted_lexeme )
					result.push_back( ch );
				else
				{
					if( !m_stream.at_end() )
					{
						typename Trait::char_t next_char = m_stream.get();

						if( next_char == const_t< Trait >::c_vertical_bar )
						{
							skip_comment = true;

							skip_one_line_comment();

							if( first_symbol )
								skip_spaces();
							else
								break;
						}
						else if( next_char == const_t< Trait >::c_sharp )
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
					else
						result.push_back( ch );
				}
			}
			else
				result.push_back( ch );

			if( m_stream.at_end() )
			{
				if( quoted_lexeme )
					throw exception_t< Trait >(
						Trait::from_ascii( "Unfinished quoted lexeme. " ) +
						Trait::from_ascii( "End of file riched. In file \"" ) +
						m_stream.file_name() +
						Trait::from_ascii( "\" on line " ) +
						Trait::to_string( line_number() ) +
						Trait::from_ascii( "." ) );
				else if( result.empty() )
					return lexeme_t< Trait >( lexeme_type_t::null,
						typename Trait::string_t() );
				else
					break;
			}

			if( !skip_comment )
				first_symbol = false;
			else
				skip_comment = false;
		}

		return lexeme_t< Trait >( lexeme_type_t::string, result );
	}

    //! \return Input stream.
    input_stream_t< Trait > & input_stream()
	{
		return m_stream;
	}

	//! \return Line number.
	typename Trait::pos_t line_number() const
	{
		return m_line_number;
	}

	//! \return Column number.
	typename Trait::pos_t column_number() const
	{
		return m_column_number;
	}

private:
	//! \return Is character a space character?
	bool is_space_char( typename Trait::char_t ch )
	{
		if( ch == const_t< Trait >::c_space || ch == const_t< Trait >::c_tab ||
			ch == const_t< Trait >::c_carriage_return ||
			ch == const_t< Trait >::c_line_feed )
				return true;
		else
			return false;
	}

	//! Skip spaces in the stream.
	void skip_spaces()
	{
		if( m_stream.at_end() )
			return;

		typename Trait::char_t ch = m_stream.get();

		while( is_space_char( ch ) )
		{
			if( m_stream.at_end() )
				return;

			ch = m_stream.get();
		}

		m_stream.put_back( ch );
	}

	//! Process back-slash sequence.
	bool process_back_slash( typename Trait::char_t & ch )
	{
		if( m_stream.at_end() )
			throw exception_t< Trait >(
				Trait::from_ascii( "Unexpected end of file. "
					"Unfinished back slash sequence. In file \"" ) +
				m_stream.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( m_stream.line_number() ) +
				Trait::from_ascii( "." ) );

		ch = m_stream.get();

		if( ch == const_t< Trait >::c_n )
			ch = const_t< Trait >::c_carriage_return;
		else if( ch == const_t< Trait >::c_t )
			ch = const_t< Trait >::c_tab;
		else if( ch == const_t< Trait >::c_r )
			ch = const_t< Trait >::c_line_feed;
		else if( ch == const_t< Trait >::c_quotes )
			ch = const_t< Trait >::c_quotes;
		else if( ch == const_t< Trait >::c_back_slash )
			ch = const_t< Trait >::c_back_slash;
		else
			return false;

		return true;
	}

	//! Skip one-line comment.
	void skip_one_line_comment()
	{
		if( !m_stream.at_end() )
		{
			typename Trait::char_t ch = m_stream.get();

			while( ch != const_t< Trait >::c_carriage_return &&
				ch != const_t< Trait >::c_line_feed &&
				!m_stream.at_end() )
					ch = m_stream.get();
		}
	}

	//! Skip multi-line comment.
	void skip_multi_line_comment()
	{
		if( !m_stream.at_end() )
		{
			typename Trait::char_t ch = m_stream.get();

			if( m_stream.at_end() )
				return;

			typename Trait::char_t next_char = m_stream.get();

			if( ch == const_t< Trait >::c_sharp &&
				next_char == const_t< Trait >::c_vertical_bar )
					return;

			while( !m_stream.at_end() )
			{
				ch = next_char;

				next_char = m_stream.get();

				if( ch == const_t< Trait >::c_sharp &&
					next_char == const_t< Trait >::c_vertical_bar )
						break;
			}
		}
	}

private:
	DISABLE_COPY( lexical_analyzer_t )

	//! Input stream.
	input_stream_t< Trait > & m_stream;
	//! Line number.
	typename Trait::pos_t m_line_number;
	//! Column number.
	typename Trait::pos_t m_column_number;
}; // class lexical_analyzer_t

} /* namespace cfgfile */

#endif // CFGFILE__LEX_HPP__INCLUDED
