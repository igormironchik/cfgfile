
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

#ifndef CFGFILE__PARSER_INFO_HPP__INCLUDED
#define CFGFILE__PARSER_INFO_HPP__INCLUDED

// cfgfile include.
#include "types.hpp"


namespace cfgfile {

//
// parser_info_t
//

//! Information about parsed file and current state of parsing.
template< typename Trait = string_trait_t >
class parser_info_t {
public:
	parser_info_t( const typename Trait::string_t & file_name,
		typename Trait::pos_t line_number,
		typename Trait::pos_t column_number )
		:	m_file_name( file_name )
		,	m_line_number( line_number )
		,	m_column_number( column_number )
	{
	}

	//! \return File name.
	const typename Trait::string_t & file_name() const
	{
		return m_file_name;
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
	//! File name.
	typename Trait::string_t m_file_name;
	//! Line number.
	typename Trait::pos_t m_line_number;
	//! Column number.
	typename Trait::pos_t m_column_number;
}; // class parser_info_t

} /* namespace cfgfile */

#endif // CFGFILE__PARSER_INFO_HPP__INCLUDED
