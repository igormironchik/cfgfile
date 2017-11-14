
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
#include "exceptions.hpp"


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
		typename Trait::string_t tmp = what.substr( 1, what.length() - 2 );

		typename Trait::string_t result;

		for( typename Trait::string_t::size_type i = 0; i < tmp.length(); ++i )
		{
			if( tmp.at( i ) == const_t< Trait >::c_back_slash )
			{
				if( i + 1 < tmp.length() )
				{
					if( tmp.at( i + 1 ) == const_t< Trait >::c_quotes )
						result.push_back( const_t< Trait >::c_quotes );
					else if( tmp.at( i + 1 ) == const_t< Trait >::c_n )
						result.push_back( const_t< Trait >::c_carriage_return );
					else if( tmp.at( i + 1 ) == const_t< Trait >::c_r )
						result.push_back( const_t< Trait >::c_line_feed );
					else if( tmp.at( i + 1 ) == const_t< Trait >::c_t )
						result.push_back( const_t< Trait >::c_tab );
					else if( tmp.at( i + 1 ) == const_t< Trait >::c_back_slash )
						result.push_back( const_t< Trait >::c_back_slash );
					else
						throw exception_t< Trait > ( Trait::from_ascii(
								"Unrecognized backslash sequence \"\\" ) +
							typename Trait::string_t( 1, tmp.at( i + 1 ) ) +
							Trait::from_ascii( "\"." ) );

					++i;
				}
				else
					throw exception_t< Trait > ( Trait::from_ascii(
						"Unfinished backslash sequence \"\\\"." ) );
			}
			else
				result.push_back( tmp.at( i ) );
		}

		return result;
	}
	else
		return what;
} // from_cfgfile_format

} /* namespace cfgfile */

#endif // CFGFILE__STRING_FORMAT_HPP__INCLUDED
