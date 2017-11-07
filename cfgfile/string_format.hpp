
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

#ifndef CFGFILE__STRING_FORMAT_HPP__INCLUDED
#define CFGFILE__STRING_FORMAT_HPP__INCLUDED

// cfgfile include.
#include "types.hpp"
#include "const.hpp"


namespace cfgfile {

//
// to_cfgfile_format
//

//! Format string to cfgfile format.
template< typename Trait >
static inline typename Trait::string_t to_cfgfile_format(
	const typename Trait::string_t & what )
{
	if( what.empty() )
		return Trait::from_ascii( "\"\"" );

	if( what.find( const_t< Trait >::c_begin_tag ) == Trait::string_t::npos &&
		what.find( const_t< Trait >::c_end_tag ) == Trait::string_t::npos &&
		what.find( const_t< Trait >::c_quotes ) == Trait::string_t::npos &&
		what.find( const_t< Trait >::c_carriage_return ) == Trait::string_t::npos &&
		what.find( const_t< Trait >::c_line_feed ) == Trait::string_t::npos &&
		what.find( const_t< Trait >::c_tab )  == Trait::string_t::npos &&
		what.find( const_t< Trait >::c_back_slash ) == Trait::string_t::npos &&
		what.find( const_t< Trait >::c_space ) == Trait::string_t::npos &&
		what.find( const_t< Trait >::c_one_line_comment ) == Trait::string_t::npos &&
		what.find( const_t< Trait >::c_start_multi_line_comment ) == Trait::string_t::npos &&
		what.find( const_t< Trait >::c_finish_multi_line_comment ) == Trait::string_t::npos )
			return what;
	else
	{
		typename Trait::string_t result;

		result.push_back( const_t< Trait >::c_quotes );

		for( const typename Trait::char_t & ch : what )
		{
			if( ch == const_t< Trait >::c_quotes )
				result.append( Trait::from_ascii( "\\\"" ) );
			else if( ch == const_t< Trait >::c_carriage_return )
				result.append( Trait::from_ascii( "\\n" ) );
			else if( ch == const_t< Trait >::c_line_feed )
				result.append( Trait::from_ascii( "\\r" ) );
			else if( ch == const_t< Trait >::c_tab )
				result.append( Trait::from_ascii( "\\t" ) );
			else if( ch == const_t< Trait >::c_back_slash )
				result.append( Trait::from_ascii( "\\\\" ) );
			else
				result.push_back( ch );
		}

		result.push_back( const_t< Trait >::c_quotes );

		return result;
	}
} // to_cfgfile_format


//
// from_cfgfile_format
//

//! Format string from cfgfile format.
template< typename Trait = string_trait_t >
static inline typename Trait::string_t from_cfgfile_format(
	const typename Trait::string_t & what )
{
	if( what.find( const_t< Trait >::c_quotes ) == 0 &&
		what.rfind( const_t< Trait >::c_quotes ) == what.length() - 1 )
	{
		typename Trait::string_t result = what.substr( 1, what.length() - 2 );

		auto replace = [] ( typename Trait::string_t & str,
			const typename Trait::string_t & old_value,
			const typename Trait::string_t & new_value )
		{
			typename Trait::string_t::size_type where = Trait::string_t::npos;

			while( ( where = str.find( old_value ) ) != Trait::string_t::npos )
				str.replace( where, old_value.length(), new_value );
		};

		replace( result, Trait::from_ascii( "\\n" ),
			typename Trait::string_t( 1, const_t< Trait >::c_carriage_return ) );
		replace( result, Trait::from_ascii( "\\\"" ),
			typename Trait::string_t( 1, const_t< Trait >::c_quotes ) );
		replace( result, Trait::from_ascii( "\\r" ),
			typename Trait::string_t( 1, const_t< Trait >::c_line_feed ) );
		replace( result, Trait::from_ascii( "\\t" ),
			typename Trait::string_t( 1, const_t< Trait >::c_tab ) );
		replace( result, Trait::from_ascii( "\\\\" ),
			typename Trait::string_t( 1, const_t< Trait >::c_back_slash ) );

		return result;
	}
	else
		return what;
} // from_cfgfile_format

} /* namespace cfgfile */

#endif // CFGFILE__STRING_FORMAT_HPP__INCLUDED
