
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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
