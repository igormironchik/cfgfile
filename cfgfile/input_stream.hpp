
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

#ifndef CFGFILE__INPUT_STREAM_HPP__INCLUDED
#define CFGFILE__INPUT_STREAM_HPP__INCLUDED

// cfgfile include.
#include "types.hpp"
#include "const.hpp"

// C++ include.
#include <stack>


namespace cfgfile {

//
// input_stream_t
//

//! Input stream for parser.
template< typename Trait = string_trait_t >
class input_stream_t final {
public:
	input_stream_t( const typename Trait::string_t & file_name,
		typename Trait::istream_t & input )
		:	m_stream( input )
		,	m_line_number( 1 )
		,	m_column_number( 1 )
		,	m_file_name( file_name )
	{
		Trait::noskipws( m_stream );
	}

	~input_stream_t()
	{
	}

	//! Get a symbol from the stream.
	typename Trait::char_t get()
	{
		if( !at_end() )
		{
			m_prev_positions.push( { m_column_number, m_line_number } );

			m_column_number += 1;

			typename Trait::char_t ch = 0x00;

			if( !m_returned_char.empty() )
			{
				ch = m_returned_char.top();

				m_returned_char.pop();

				if( is_new_line( ch ) )
				{
					m_line_number += 1;
					m_column_number = 1;
				}

				return ch;
			}

			m_stream >> ch;

			if( is_new_line( ch ) )
			{
				m_line_number += 1;
				m_column_number = 1;
			}

			return ch;
		}
		else
			return 0x00;
	}

	//! Put symbol back in the stream.
	void put_back( typename Trait::char_t ch )
	{
		if( !m_prev_positions.empty() )
		{
			const auto prev = m_prev_positions.top();
			m_prev_positions.pop();

			m_column_number = prev.m_column_number;
			m_line_number = prev.m_line_number;

			m_returned_char.push( ch );
		}
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

	//! \return Is stream at end?
	bool at_end() const
	{
		if( m_returned_char.empty() )
			return Trait::is_at_end( m_stream );
		else
			return false;
	}

	//! \return File name.
	const typename Trait::string_t & file_name() const
	{
		return m_file_name;
	}

private:
	bool is_new_line( typename Trait::char_t & ch )
	{
		if( ch == const_t< Trait >::c_carriage_return )
			return true;
		else if( ch == const_t< Trait >::c_line_feed )
		{
			typename Trait::char_t next_char = 0x00;

			next_char = simple_get();

			if( next_char == const_t< Trait >::c_carriage_return )
			{
				ch = next_char;

				return true;
			}
			else
			{
				m_returned_char.push( next_char );

				return true;
			}
		}
		else
			return false;
	}

	typename Trait::char_t simple_get()
	{
		if( !m_returned_char.empty() )
		{
			auto ch = m_returned_char.top();

			m_returned_char.pop();

			return ch;
		}
		else
		{
			typename Trait::char_t ch = 0x00;

			m_stream >> ch;

			return ch;
		}
	}

private:
	DISABLE_COPY( input_stream_t )

	struct position_t {
		typename Trait::pos_t m_column_number;
		typename Trait::pos_t m_line_number;
	};

	//! Underline input stream.
	typename Trait::istream_t & m_stream;
	//! Line number.
	typename Trait::pos_t m_line_number;
	//! Column number.
	typename Trait::pos_t m_column_number;
	//! Previous positions.
	std::stack< position_t > m_prev_positions;
	//! File name.
	typename Trait::string_t m_file_name;
	//! Returned char.
	std::stack< typename Trait::char_t > m_returned_char;
}; // class input_stream_t

} /* namespace cfgfile */

#endif // CFGFILE__INPUT_STREAM_HPP__INCLUDED
