
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef CFGFILE__INPUT_STREAM_HPP__INCLUDED
#define CFGFILE__INPUT_STREAM_HPP__INCLUDED

// cfgfile include.
#include "types.hpp"
#include "const.hpp"

// C++ include.
#include <stack>


namespace cfgfile {

//! Size of the buffer.
static const std::size_t c_buff_size = 512;


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
		,	m_buf_pos( 0 )
		,	m_stream_pos( 0 )
	{
		Trait::noskipws( m_stream );

		m_stream_size = Trait::size_of_file( m_stream );

		if( m_stream_size < 0 )
			m_stream_size = 0;

		Trait::fill_buf( m_stream, m_buf, c_buff_size, m_stream_pos, m_stream_size );
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

			typename Trait::char_t ch( 0x00 );

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

			if( m_buf_pos == c_buff_size )
			{
				Trait::fill_buf( m_stream, m_buf, c_buff_size, m_stream_pos, m_stream_size );
				m_buf_pos = 0;
			}

			ch = m_buf[ m_buf_pos ];

			++m_buf_pos;

			if( is_new_line( ch ) )
			{
				m_line_number += 1;
				m_column_number = 1;
			}

			return ch;
		}
		else
			return typename Trait::char_t( 0x00 );
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
			return ( Trait::is_at_end( m_stream ) && m_buf_pos == (size_t) m_buf.size() );
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
			typename Trait::char_t next_char( 0x00 );

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
			typename Trait::char_t ch( 0x00 );

			if( m_buf_pos == c_buff_size )
			{
				Trait::fill_buf( m_stream, m_buf, c_buff_size, m_stream_pos, m_stream_size );
				m_buf_pos = 0;
			}

			ch = m_buf[ m_buf_pos ];

			++m_buf_pos;

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
	//! Buffer.
	typename Trait::buf_t m_buf;
	//! Buffer position.
	std::size_t m_buf_pos;
	//! Size of the stream.
	typename Trait::pos_t m_stream_size;
	//! Current position in the stream.
	typename Trait::pos_t m_stream_pos;
}; // class input_stream_t

} /* namespace cfgfile */

#endif // CFGFILE__INPUT_STREAM_HPP__INCLUDED
