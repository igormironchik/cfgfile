
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
	static T from_string( const parser_info_t< Trait > &, const string_t & )
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
	static int from_string( const parser_info_t< Trait > & info, const string_t & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		bool ok = false;
		int result = ((QString)value).toInt( &ok );

		if( !ok )
			throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
				value + Trait::from_ascii( "\". In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );
		else
			return result;
#else
		try {
			std::size_t pos = 0;

			int result = std::stoi( value, &pos );

			if( pos != value.length() )
				throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
					value + Trait::from_ascii( "\". In file \"" ) +
					info.file_name() + Trait::from_ascii( "\" on line " ) +
					Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );

			return result;
		}
		catch( const std::exception & )
		{
			throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
				value + Trait::from_ascii( "\". In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );
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
	static unsigned int from_string( const parser_info_t< Trait > & info, const string_t & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		bool ok = false;
		unsigned int result = ((QString)value).toUInt( &ok );

		if( !ok )
			throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
				value + Trait::from_ascii( "\". In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );
		else
			return result;
#else
		try {
			std::size_t pos = 0;

			int result = std::stoi( value, &pos );

			if( pos != value.length() || result < 0 )
				throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
					value + Trait::from_ascii( "\". In file \"" ) +
					info.file_name() + Trait::from_ascii( "\" on line " ) +
					Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );

			return (unsigned int) result;
		}
		catch( const std::exception & )
		{
			throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
				value + Trait::from_ascii( "\". In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );
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
	static long from_string( const parser_info_t< Trait > & info, const string_t & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		bool ok = false;
		long result = ((QString)value).toLong( &ok );

		if( !ok )
			throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
				value + Trait::from_ascii( "\". In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );
		else
			return result;
#else
		try {
			std::size_t pos = 0;

			long result = std::stol( value, &pos );

			if( pos != value.length() )
				throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
					value + Trait::from_ascii( "\". In file \"" ) +
					info.file_name() + Trait::from_ascii( "\" on line " ) +
					Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );

			return (unsigned int) result;
		}
		catch( const std::exception & )
		{
			throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
				value + Trait::from_ascii( "\". In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );
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
	static unsigned long from_string( const parser_info_t< Trait > & info, const string_t & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		bool ok = false;
		unsigned long result = ((QString)value).toULong( &ok );

		if( !ok )
			throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
				value + Trait::from_ascii( "\". In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );
		else
			return result;
#else
		try {
			std::size_t pos = 0;

			long result = std::stol( value, &pos );

			if( pos != value.length() || result < 0 )
				throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
					value + Trait::from_ascii( "\". In file \"" ) +
					info.file_name() + Trait::from_ascii( "\" on line " ) +
					Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );

			return (unsigned long) result;
		}
		catch( const std::exception & )
		{
			throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
				value + Trait::from_ascii( "\". In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );
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
	static long long from_string( const parser_info_t< Trait > & info, const string_t & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		bool ok = false;
		long long result = ((QString)value).toLongLong( &ok );

		if( !ok )
			throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
				value + Trait::from_ascii( "\". In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );
		else
			return result;
#else
		try {
			std::size_t pos = 0;

			long long result = std::stoll( value, &pos );

			if( pos != value.length() )
				throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
					value + Trait::from_ascii( "\". In file \"" ) +
					info.file_name() + Trait::from_ascii( "\" on line " ) +
					Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );

			return (long long) result;
		}
		catch( const std::exception & )
		{
			throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
				value + Trait::from_ascii( "\". In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );
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
	static unsigned long long from_string( const parser_info_t< Trait > & info, const string_t & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		bool ok = false;
		unsigned long long result = ((QString)value).toULongLong( &ok );

		if( !ok )
			throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
				value + Trait::from_ascii( "\". In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );
		else
			return result;
#else
		try {
			std::size_t pos = 0;

			long long result = std::stoll( value, &pos );

			if( pos != value.length() )
				throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
					value + Trait::from_ascii( "\". In file \"" ) +
					info.file_name() + Trait::from_ascii( "\" on line " ) +
					Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );

			return (unsigned long long) result;
		}
		catch( const std::exception & )
		{
			throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
				value + Trait::from_ascii( "\". In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );
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
	static double from_string( const parser_info_t< Trait > & info, const string_t & value )
	{
#ifdef CFGFILE_QSTRING_BUILD
		bool ok = false;
		double result = ((QString)value).toDouble( &ok );

		if( !ok )
			throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
				value + Trait::from_ascii( "\". In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );
		else
			return result;
#else
		try {
			std::size_t pos = 0;

			double result = std::stod( value, &pos );

			if( pos != value.length() )
				throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
					value + Trait::from_ascii( "\". In file \"" ) +
					info.file_name() + Trait::from_ascii( "\" on line " ) +
					Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );

			return (unsigned int) result;
		}
		catch( const std::exception & )
		{
			throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
				value + Trait::from_ascii( "\". In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );
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
	static string_t from_string( const parser_info_t< Trait > &, const string_t & value )
	{
		return value;
	}
}; // class format_t< string_t >

#ifdef CFGFILE_QSTRING_BUILD
template<>
class format_t< QString > {
public:
	//! Format value to string.
	static QString to_string( const QString & value )
	{
		return value;
	}

	//! Format value from string.
	static QString from_string( const parser_info_t< Trait > &, const QString & value )
	{
		return value;
	}
}; // class format_t< QString >
#endif


static const string_t c_on = Trait::from_ascii( "on" );
static const string_t c_off = Trait::from_ascii( "off" );
static const string_t c_true = Trait::from_ascii( "true" );
static const string_t c_false = Trait::from_ascii( "false" );
static const string_t c_1 = Trait::from_ascii( "1" );
static const string_t c_0 = Trait::from_ascii( "0" );

template<>
class format_t< bool > {
public:
	//! Format value to string.
	static string_t to_string( const bool & value )
	{
		return ( value ? Trait::from_ascii( "true" ) : Trait::from_ascii( "false" ) );
	}

	//! Format value from string.
	static bool from_string( const parser_info_t< Trait > & info, const string_t & value )
	{
		if( value == c_on || value == c_true || value == c_1 )
			return true;
		else if( value == c_off || value == c_false || value == c_0 )
			return false;
		else
			throw exception_t< Trait >( string_t( Trait::from_ascii( "Invalid value: \"" ) ) +
				value + Trait::from_ascii( "\". In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) + Trait::from_ascii( "." ) );
	}
}; // class format_t< bool >

} /* namespace cfgfile */

#endif // CFGFILE__FORMAT_HPP__INCLUDED
