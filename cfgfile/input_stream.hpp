
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


namespace cfgfile {

//
// input_stream_t
//

//! Input stream for parser.
class input_stream_t final {
public:
	input_stream_t( const string_t & file_name,
		istream_t & input )
		:	m_stream( input )
		,	m_line_number( 0 )
		,	m_column_number( 0 )
		,	m_file_name( file_name )
		,	m_returned_char( 0 )
	{
#ifndef CFGFILE_QSTRING_BUILD
		m_stream >> std::noskipws;
#endif
	}

	~input_stream_t()
	{
	}

	//! Get a symbol from the stream.
	char_t get()
	{
		m_column_number += 1;

		char_t ch = 0x00;

		if( m_returned_char != 0x00 )
		{
			ch = m_returned_char;

			m_returned_char = 0x00;

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

	//! Put symbol back in the stream.
	void put_back( char_t ch )
	{
		m_column_number -= 1;

		if( ch == c_carriage_return || ch == c_line_feed )
			m_line_number -= 1;

		m_returned_char = ch;
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

	//! \return Is stream at end?
	bool at_end() const
	{
		if( m_returned_char == 0x00 )
#ifdef CFGFILE_QSTRING_BUILD
			return m_stream.atEnd();
#else
			return m_stream.eof();
#endif
		else
			return false;
	}

	//! \return File name.
	const string_t & file_name() const
	{
		return m_file_name;
	}

private:
	bool is_new_line( char_t & ch )
	{
		if( ch == c_carriage_return )
			return true;
		else if( ch == c_line_feed )
		{
			char_t next_char = 0x00;

			m_stream >> next_char;

			if( next_char == c_carriage_return )
			{
				ch = next_char;

				return true;
			}
			else
			{
				m_returned_char = next_char;

				return true;
			}
		}
		else
			return false;
	}

private:
	DISABLE_COPY( input_stream_t )

	//! Underline input stream.
	istream_t & m_stream;
	//! Line number.
	pos_t m_line_number;
	//! Column number.
	pos_t m_column_number;
	//! File name.
	string_t m_file_name;
	//! Returned char.
	char_t m_returned_char;
}; // class input_stream_t

} /* namespace cfgfile */

#endif // CFGFILE__INPUT_STREAM_HPP__INCLUDED
