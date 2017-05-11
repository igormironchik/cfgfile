
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

#ifndef CFGFILE__CONST_HPP__INCLUDED
#define CFGFILE__CONST_HPP__INCLUDED

// cfgfile include.
#include "types.hpp"


namespace cfgfile {

//
// const_t
//

template< typename Trait >
struct const_t {
	static const Trait::char_t c_begin_tag = Trait::from_ascii( '{' );
	static const Trait::char_t c_end_tag = Trait::from_ascii( '}' );
	static const Trait::char_t c_quotes = Trait::from_ascii( '"' );
	static const Trait::char_t c_n = Trait::from_ascii( 'n' );
	static const Trait::char_t c_t = Trait::from_ascii( 't' );
	static const Trait::char_t c_r = Trait::from_ascii( 'r' );
	static const Trait::char_t c_back_slash = Trait::from_ascii( '\\' );
	static const Trait::char_t c_space = Trait::from_ascii( ' ' );
	static const Trait::char_t c_tab = Trait::from_ascii( '\t' );
	static const Trait::char_t c_carriage_return = Trait::from_ascii( '\n' );
	static const Trait::char_t c_line_feed = Trait::from_ascii( '\r' );
	static const Trait::char_t c_vertical_bar = Trait::from_ascii( '|' );
	static const Trait::char_t c_sharp = Trait::from_ascii( '#' );
	static const Trait::string_t c_one_line_comment = Trait::from_ascii( "||" );
	static const Trait::string_t c_start_multi_line_comment = Trait::from_ascii( "|#" );
	static const Trait::string_t c_finish_multi_line_comment = Trait::from_ascii( "#|" );
}; // struct const_t

} /* namespace cfgfile */

#endif // CFGFILE__CONST_HPP__INCLUDED
