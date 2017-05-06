
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
// tocfgfileFormat
//

//! Format string to cfgfile format.
string_t to_cfgfile_format( const string_t & what )
{
	if( what.empty() )
		return SL( "\"\"" );

	if( what.find( c_begin_tag ) == string_t::npos &&
		what.find( c_end_tag ) == string_t::npos &&
		what.find( c_quotes ) == string_t::npos &&
		what.find( c_carriage_return ) == string_t::npos &&
		what.find( c_line_feed ) == string_t::npos &&
		what.find( c_tab )  == string_t::npos &&
		what.find( c_back_slash ) == string_t::npos &&
		what.find( c_space ) == string_t::npos &&
		what.find( c_one_line_comment ) == string_t::npos &&
		what.find( c_start_multi_line_comment ) == string_t::npos &&
		what.find( c_finish_multi_line_comment ) == string_t::npos )
			return what;
	else
	{
		string_t result;

		result.push_back( c_quotes );

		for( const char_t & ch : what )
		{
			if( ch == c_quotes )
				result.push_back( SL( "\\\"" ) );
			else if( ch == c_carriage_return )
				result.push_back( SL( "\\n" ) );
			else if( ch == c_line_feed )
				result.push_back( SL( "\\r" ) );
			else if( ch == c_tab )
				result.push_back( SL( "\\t" ) );
			else if( ch == c_back_slash )
				result.push_back( SL( "\\\\" ) );
			else
				result.push_back( ch );
		}

		result.push_back( c_quotes );

		return result;
	}
}


//
// fromcfgfileFormat
//

//! Format string from cfgfile format.
string_t from_cfgfile_format( const string_t & what )
{
	if( what.find( c_quotes ) == 0 &&
		what.rfind( c_quotes ) == what.length() - 1 )
	{
		string_t result = what.substr( 1, what.length() - 2 );

		auto replace = [] ( string_t & str, const string_t & old_value,
			const string_t & new_value )
		{
			pos_t where = string_t::npos;

			while( ( where = str.find( old_value ) ) != string_t::npos )
				str.replace( where, old_value.length(), new_value );
		};

		replace( result, c_carriage_return, SL( "\\n" ) );
		replace( result, c_quotes, SL( "\\\"" ) );
		replace( result, c_line_feed, SL( "\\r" ) );
		replace( result, c_tab, SL( "\\t" ) );
		replace( result, c_back_slash, SL( "\\\\" ) );

		return result;
	}
	else
		return what;
}

} /* namespace cfgfile */

#endif // CFGFILE__STRING_FORMAT_HPP__INCLUDED
