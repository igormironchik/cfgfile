
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

#ifndef CFGFILE__FORMAT_HPP__INCLUDED
#define CFGFILE__FORMAT_HPP__INCLUDED

// cfgfile include.
#include "parser_info.hpp"
#include "exceptions.hpp"
#include "types.hpp"

#ifdef CFGFILE_QSTRING_BUILD
// Qt include.
#include <QString>
#endif


namespace cfgfile {

//
// format_t
//

//! Format template value to the string and back.
template< class T >
class format_t {
public:
	//! Format value to string.
	static string_t to_string( const T & )
	{
		return string_t();
	}

	//! Format value from string.
	static T from_string( const parser_info_t &, const string_t & )
	{
		return T();
	}
}; // class format_t


template<>
class format_t< int > {
public:
	//! Format value to string.
	static string_t to_string( const int & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		return QString::number( value );
#elif defined( CFGFILE_WSTRING_BUILD )
		return std::to_wstring( value );
#else
		return std::to_string( value );
#endif
	}

	//! Format value from string.
	static int from_string( const parser_info_t & info, const string_t & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		bool ok = false;
		int result = value.toInt( &ok );

		if( !ok )
			throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
				value + SL( "\". In file \"" ) +
				info.file_name() + SL( "\" on line " ) +
				info.line_number() + SL( "." ) );
		else
			return result;
#else
		try {
			std::size_t pos = 0;

			int result = std::stoi( value, &pos );

			if( pos != value.length() )
				throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
					value + SL( "\". In file \"" ) +
					info.file_name() + SL( "\" on line " ) +
					info.line_number() + SL( "." ) );

			return result;
		}
		catch( const std::exception & )
		{
			throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
				value + SL( "\". In file \"" ) +
				info.file_name() + SL( "\" on line " ) +
				info.line_number() + SL( "." ) );
		}
#endif
	}
}; // class format_t< int >


template<>
class format_t< unsigned int > {
public:
	//! Format value to string.
	static string_t to_string( const unsigned int & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		return QString::number( value );
#elif defined( CFGFILE_WSTRING_BUILD )
		return std::to_wstring( value );
#else
		return std::to_string( value );
#endif
	}

	//! Format value from string.
	static unsigned int from_string( const parser_info_t & info, const string_t & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		bool ok = false;
		unsigned int result = value.toUInt( &ok );

		if( !ok )
			throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
				value + SL( "\". In file \"" ) +
				info.file_name() + SL( "\" on line " ) +
				info.line_number() + SL( "." ) );
		else
			return result;
#else
		try {
			std::size_t pos = 0;

			int result = std::stoi( value, &pos );

			if( pos != value.length() || result < 0 )
				throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
					value + SL( "\". In file \"" ) +
					info.file_name() + SL( "\" on line " ) +
					info.line_number() + SL( "." ) );

			return (unsigned int) result;
		}
		catch( const std::exception & )
		{
			throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
				value + SL( "\". In file \"" ) +
				info.file_name() + SL( "\" on line " ) +
				info.line_number() + SL( "." ) );
		}
#endif
	}
}; // class format_t< unsigned int >


template<>
class format_t< long > {
public:
	//! Format value to string.
	static string_t to_string( const long & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		return QString::number( value );
#elif defined( CFGFILE_WSTRING_BUILD )
		return std::to_wstring( value );
#else
		return std::to_string( value );
#endif
	}

	//! Format value from string.
	static long from_string( const parser_info_t & info, const string_t & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		bool ok = false;
		long result = value.toLong( &ok );

		if( !ok )
			throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
				value + SL( "\". In file \"" ) +
				info.file_name() + SL( "\" on line " ) +
				info.line_number() + SL( "." ) );
		else
			return result;
#else
		try {
			std::size_t pos = 0;

			long result = std::stol( value, &pos );

			if( pos != value.length() )
				throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
					value + SL( "\". In file \"" ) +
					info.file_name() + SL( "\" on line " ) +
					info.line_number() + SL( "." ) );

			return (unsigned int) result;
		}
		catch( const std::exception & )
		{
			throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
				value + SL( "\". In file \"" ) +
				info.file_name() + SL( "\" on line " ) +
				info.line_number() + SL( "." ) );
		}
#endif
	}
}; // class format_t< long >


template<>
class format_t< unsigned long > {
public:
	//! Format value to string.
	static string_t to_string( const unsigned long & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		return QString::number( value );
#elif defined( CFGFILE_WSTRING_BUILD )
		return std::to_wstring( value );
#else
		return std::to_string( value );
#endif
	}

	//! Format value from string.
	static unsigned long from_string( const parser_info_t & info, const string_t & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		bool ok = false;
		unsigned long result = value.toULong( &ok );

		if( !ok )
			throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
				value + SL( "\". In file \"" ) +
				info.file_name() + SL( "\" on line " ) +
				info.line_number() + SL( "." ) );
		else
			return result;
#else
		try {
			std::size_t pos = 0;

			long result = std::stol( value, &pos );

			if( pos != value.length() || result < 0 )
				throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
					value + SL( "\". In file \"" ) +
					info.file_name() + SL( "\" on line " ) +
					info.line_number() + SL( "." ) );

			return (unsigned long) result;
		}
		catch( const std::exception & )
		{
			throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
				value + SL( "\". In file \"" ) +
				info.file_name() + SL( "\" on line " ) +
				info.line_number() + SL( "." ) );
		}
#endif
	}
}; // class format_t< unsigned long >


template<>
class format_t< long long > {
public:
	//! Format value to string.
	static string_t to_string( const long long & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		return QString::number( value );
#elif defined( CFGFILE_WSTRING_BUILD )
		return std::to_wstring( value );
#else
		return std::to_string( value );
#endif
	}

	//! Format value from string.
	static long long from_string( const parser_info_t & info, const string_t & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		bool ok = false;
		long long result = value.toLongLong( &ok );

		if( !ok )
			throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
				value + SL( "\". In file \"" ) +
				info.file_name() + SL( "\" on line " ) +
				info.line_number() + SL( "." ) );
		else
			return result;
#else
		try {
			std::size_t pos = 0;

			long long result = std::stoll( value, &pos );

			if( pos != value.length() )
				throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
					value + SL( "\". In file \"" ) +
					info.file_name() + SL( "\" on line " ) +
					info.line_number() + SL( "." ) );

			return (long long) result;
		}
		catch( const std::exception & )
		{
			throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
				value + SL( "\". In file \"" ) +
				info.file_name() + SL( "\" on line " ) +
				info.line_number() + SL( "." ) );
		}
#endif
	}
}; // class format_t< qlonglong >


template<>
class format_t< unsigned long long > {
public:
	//! Format value to string.
	static string_t to_string( const unsigned long long & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		return QString::number( value );
#elif defined( CFGFILE_WSTRING_BUILD )
		return std::to_wstring( value );
#else
		return std::to_string( value );
#endif
	}

	//! Format value from string.
	static unsigned long long from_string( const parser_info_t & info, const string_t & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		bool ok = false;
		unsigned long long result = value.toULongLong( &ok );

		if( !ok )
			throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
				value + SL( "\". In file \"" ) +
				info.file_name() + SL( "\" on line " ) +
				info.line_number() + SL( "." ) );
		else
			return result;
#else
		try {
			std::size_t pos = 0;

			long long result = std::stoll( value, &pos );

			if( pos != value.length() || pos < 0 )
				throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
					value + SL( "\". In file \"" ) +
					info.file_name() + SL( "\" on line " ) +
					info.line_number() + SL( "." ) );

			return (unsigned long long) result;
		}
		catch( const std::exception & )
		{
			throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
				value + SL( "\". In file \"" ) +
				info.file_name() + SL( "\" on line " ) +
				info.line_number() + SL( "." ) );
		}
#endif
	}
}; // class format_t< unsigned long long >


template<>
class format_t< double > {
public:
	//! Format value to string.
	static string_t to_string( const double & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		return QString::number( value );
#elif defined( CFGFILE_WSTRING_BUILD )
		return std::to_wstring( value );
#else
		return std::to_string( value );
#endif
	}

	//! Format value from string.
	static double from_string( const parser_info_t & info, const string_t & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		bool ok = false;
		double result = value.toDouble( &ok );

		if( !ok )
			throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
				value + SL( "\". In file \"" ) +
				info.file_name() + SL( "\" on line " ) +
				info.line_number() + SL( "." ) );
		else
			return result;
#else
		try {
			std::size_t pos = 0;

			double result = std::stod( value, &pos );

			if( pos != value.length() )
				throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
					value + SL( "\". In file \"" ) +
					info.file_name() + SL( "\" on line " ) +
					info.line_number() + SL( "." ) );

			return (unsigned int) result;
		}
		catch( const std::exception & )
		{
			throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
				value + SL( "\". In file \"" ) +
				info.file_name() + SL( "\" on line " ) +
				info.line_number() + SL( "." ) );
		}
#endif
	}
}; // class format_t< double >


template<>
class format_t< string_t > {
public:
	//! Format value to string.
	static string_t to_string( const string_t & value )
	{
		return value;
	}

	//! Format value from string.
	static string_t from_string( const parser_info_t &, const string_t & value )
	{
		return value;
	}
}; // class format_t< string_t >


static const string_t c_on = SL( "on" );
static const string_t c_off = SL( "off" );
static const string_t c_true = SL( "true" );
static const string_t c_false = SL( "false" );
static const string_t c_1 = SL( "1" );
static const string_t c_0 = SL( "0" );

template<>
class format_t< bool > {
public:
	//! Format value to string.
	static string_t to_string( const bool & value )
	{
		return ( value ? SL( "true" ) : SL( "false" ) );
	}

	//! Format value from string.
	static bool from_string( const parser_info_t & info, const string_t & value )
	{
		if( value == c_on || value == c_true || value == c_1 )
			return true;
		else if( value == c_off || value == c_false || value == c_0 )
			return false;
		else
			throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
				value + SL( "\". In file \"" ) +
				info.file_name() + SL( "\" on line " ) +
				info.line_number() + SL( "." ) );
	}
}; // class format_t< bool >

} /* namespace cfgfile */

#endif // CFGFILE__FORMAT_HPP__INCLUDED
