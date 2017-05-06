
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

static const char_t c_begin_tag = SL('{' );
static const char_t c_end_tag = SL( '}' );
static const char_t c_quotes = SL( '"' );
static const char_t c_n = SL( 'n' );
static const char_t c_t = SL( 't' );
static const char_t c_r = SL( 'r' );
static const char_t c_back_slash = SL( '\\' );
static const char_t c_space = SL( ' ' );
static const char_t c_tab = SL( '\t' );
static const char_t c_carriage_return = SL( '\n' );
static const char_t c_line_feed = SL( '\r' );
static const char_t c_vertical_bar = SL( '|' );
static const char_t c_sharp = SL( '#' );

} /* namespace cfgfile */

#endif // CFGFILE__CONST_HPP__INCLUDED
