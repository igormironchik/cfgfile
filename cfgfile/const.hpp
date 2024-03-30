
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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
	static const typename Trait::char_t c_begin_tag;
	static const typename Trait::char_t c_end_tag;
	static const typename Trait::char_t c_quotes;
	static const typename Trait::char_t c_n;
	static const typename Trait::char_t c_t;
	static const typename Trait::char_t c_r;
	static const typename Trait::char_t c_back_slash;
	static const typename Trait::char_t c_space;
	static const typename Trait::char_t c_tab;
	static const typename Trait::char_t c_carriage_return;
	static const typename Trait::char_t c_line_feed;
	static const typename Trait::char_t c_vertical_bar;
	static const typename Trait::char_t c_sharp;

	static const typename Trait::string_t c_one_line_comment;
	static const typename Trait::string_t c_start_multi_line_comment;
	static const typename Trait::string_t c_finish_multi_line_comment;

	static const typename Trait::string_t c_on;
	static const typename Trait::string_t c_off;
	static const typename Trait::string_t c_true;
	static const typename Trait::string_t c_false;
	static const typename Trait::string_t c_1;
	static const typename Trait::string_t c_0;
	static const typename Trait::string_t c_yes;
	static const typename Trait::string_t c_no;
}; // struct const_t

template< typename Trait >
const typename Trait::char_t const_t< Trait >::c_begin_tag =
	Trait::from_ascii( '{' );

template< typename Trait >
const typename Trait::char_t const_t< Trait >::c_end_tag =
	Trait::from_ascii( '}' );

template< typename Trait >
const typename Trait::char_t const_t< Trait >::c_quotes =
	Trait::from_ascii( '"' );

template< typename Trait >
const typename Trait::char_t const_t< Trait >::c_n =
	Trait::from_ascii( 'n' );

template< typename Trait >
const typename Trait::char_t const_t< Trait >::c_t =
	Trait::from_ascii( 't' );

template< typename Trait >
const typename Trait::char_t const_t< Trait >::c_r =
	Trait::from_ascii( 'r' );

template< typename Trait >
const typename Trait::char_t const_t< Trait >::c_back_slash =
	Trait::from_ascii( '\\' );

template< typename Trait >
const typename Trait::char_t const_t< Trait >::c_space =
	Trait::from_ascii( ' ' );

template< typename Trait >
const typename Trait::char_t const_t< Trait >::c_tab =
	Trait::from_ascii( '\t' );

template< typename Trait >
const typename Trait::char_t const_t< Trait >::c_carriage_return =
	Trait::from_ascii( '\n' );

template< typename Trait >
const typename Trait::char_t const_t< Trait >::c_line_feed =
	Trait::from_ascii( '\r' );

template< typename Trait >
const typename Trait::char_t const_t< Trait >::c_vertical_bar =
	Trait::from_ascii( '|' );

template< typename Trait >
const typename Trait::char_t const_t< Trait >::c_sharp =
	Trait::from_ascii( '#' );

template< typename Trait >
const typename Trait::string_t const_t< Trait >::c_one_line_comment =
	Trait::from_ascii( "||" );

template< typename Trait >
const typename Trait::string_t const_t< Trait >::c_start_multi_line_comment =
	Trait::from_ascii( "|#" );

template< typename Trait >
const typename Trait::string_t const_t< Trait >::c_finish_multi_line_comment =
	Trait::from_ascii( "#|" );

template< typename Trait >
const typename Trait::string_t const_t< Trait >::c_on =
	Trait::from_ascii( "on" );

template< typename Trait >
const typename Trait::string_t const_t< Trait >::c_off =
	Trait::from_ascii( "off" );

template< typename Trait >
const typename Trait::string_t const_t< Trait >::c_true =
	Trait::from_ascii( "true" );

template< typename Trait >
const typename Trait::string_t const_t< Trait >::c_false =
	Trait::from_ascii( "false" );

template< typename Trait >
const typename Trait::string_t const_t< Trait >::c_1 =
	Trait::from_ascii( "1" );

template< typename Trait >
const typename Trait::string_t const_t< Trait >::c_0 =
	Trait::from_ascii( "0" );

template< typename Trait >
const typename Trait::string_t const_t< Trait >::c_yes =
	Trait::from_ascii( "yes" );

template< typename Trait >
const typename Trait::string_t const_t< Trait >::c_no =
	Trait::from_ascii( "no" );

} /* namespace cfgfile */

#endif // CFGFILE__CONST_HPP__INCLUDED
