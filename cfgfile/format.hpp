
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
#include <cfgfile/private/ParserInfo>
#include <cfgfile/Exceptions>

// Qt include.
#include <QtCore/QString>


namespace cfgfile {

//
// Format
//

//! Format template value to the string and back.
template< class T >
class Format {
public:
	//! Format value to string.
	static QString toString( const T & value )
	{
		Q_UNUSED( value )

		return QString();
	}

	//! Format value from string.
	static T fromString( const ParserInfo & info, const QString & value )
	{
		Q_UNUSED( info )
		Q_UNUSED( value )

		return T();
	}
}; // class Format


template<>
class Format< int > {
public:
	//! Format value to string.
	static QString toString( const int & value )
	{
		return QString::number( value );
	}

	//! Format value from string.
	static int fromString( const ParserInfo & info, const QString & value )
	{
		bool ok = false;
		int result = value.toInt( &ok );

		if( !ok )
			throw Exception( QString( "Invalid value: \"%1\". "
				"In file \"%2\" on line %3." )
					.arg( value )
					.arg( info.fileName() )
					.arg( info.lineNumber() ) );
		else
			return result;
	}
}; // class Format< int >


template<>
class Format< uint > {
public:
	//! Format value to string.
	static QString toString( const uint & value )
	{
		return QString::number( value );
	}

	//! Format value from string.
	static uint fromString( const ParserInfo & info, const QString & value )
	{
		bool ok = false;
		uint result = value.toUInt( &ok );

		if( !ok )
			throw Exception( QString( "Invalid value: \"%1\". "
				"In file \"%2\" on line %3." )
					.arg( value )
					.arg( info.fileName() )
					.arg( info.lineNumber() ) );
		else
			return result;
	}
}; // class Format< uint >


template<>
class Format< long > {
public:
	//! Format value to string.
	static QString toString( const long & value )
	{
		return QString::number( value );
	}

	//! Format value from string.
	static long fromString( const ParserInfo & info, const QString & value )
	{
		bool ok = false;
		long result = value.toLong( &ok );

		if( !ok )
			throw Exception( QString( "Invalid value: \"%1\". "
				"In file \"%2\" on line %3." )
					.arg( value )
					.arg( info.fileName() )
					.arg( info.lineNumber() ) );
		else
			return result;
	}
}; // class Format< long >


template<>
class Format< ulong > {
public:
	//! Format value to string.
	static QString toString( const ulong & value )
	{
		return QString::number( value );
	}

	//! Format value from string.
	static ulong fromString( const ParserInfo & info, const QString & value )
	{
		bool ok = false;
		ulong result = value.toULong( &ok );

		if( !ok )
			throw Exception( QString( "Invalid value: \"%1\". "
				"In file \"%2\" on line %3." )
					.arg( value )
					.arg( info.fileName() )
					.arg( info.lineNumber() ) );
		else
			return result;
	}
}; // class Format< ulong >


template<>
class Format< qlonglong > {
public:
	//! Format value to string.
	static QString toString( const qlonglong & value )
	{
		return QString::number( value );
	}

	//! Format value from string.
	static qlonglong fromString( const ParserInfo & info, const QString & value )
	{
		bool ok = false;
		qlonglong result = value.toLongLong( &ok );

		if( !ok )
			throw Exception( QString( "Invalid value: \"%1\". "
				"In file \"%2\" on line %3." )
					.arg( value )
					.arg( info.fileName() )
					.arg( info.lineNumber() ) );
		else
			return result;
	}
}; // class Format< qlonglong >


template<>
class Format< qulonglong > {
public:
	//! Format value to string.
	static QString toString( const qulonglong & value )
	{
		return QString::number( value );
	}

	//! Format value from string.
	static qulonglong fromString( const ParserInfo & info, const QString & value )
	{
		bool ok = false;
		qulonglong result = value.toULongLong( &ok );

		if( !ok )
			throw Exception( QString( "Invalid value: \"%1\". "
				"In file \"%2\" on line %3." )
					.arg( value )
					.arg( info.fileName() )
					.arg( info.lineNumber() ) );
		else
			return result;
	}
}; // class Format< qulonglong >


template<>
class Format< double > {
public:
	//! Format value to string.
	static QString toString( const double & value )
	{
		return QString::number( value );
	}

	//! Format value from string.
	static double fromString( const ParserInfo & info, const QString & value )
	{
		bool ok = false;
		double result = value.toDouble( &ok );

		if( !ok )
			throw Exception( QString( "Invalid value: \"%1\". "
				"In file \"%2\" on line %3." )
					.arg( value )
					.arg( info.fileName() )
					.arg( info.lineNumber() ) );
		else
			return result;
	}
}; // class Format< double >


template<>
class Format< QString > {
public:
	//! Format value to string.
	static QString toString( const QString & value )
	{
		return value;
	}

	//! Format value from string.
	static QString fromString( const ParserInfo & info, const QString & value )
	{
		Q_UNUSED( info )

		return value;
	}
}; // class Format< double >


static const QString c_on = QLatin1String( "on" );
static const QString c_off = QLatin1String( "off" );
static const QString c_true = QLatin1String( "true" );
static const QString c_false = QLatin1String( "false" );
static const QString c_1 = QLatin1String( "1" );
static const QString c_0 = QLatin1String( "0" );

template<>
class Format< bool > {
public:
	//! Format value to string.
	static QString toString( const bool & value )
	{
		return ( value ? QLatin1String( "true" ) : QLatin1String( "false" ) );
	}

	//! Format value from string.
	static bool fromString( const ParserInfo & info, const QString & value )
	{
		if( value == c_on || value == c_true || value == c_1 )
			return true;
		else if( value == c_off || value == c_false || value == c_0 )
			return false;
		else
			throw Exception( QString( "Invalid value: \"%1\". "
				"In file \"%2\" on line %3." )
					.arg( value )
					.arg( info.fileName() )
					.arg( info.lineNumber() ) );
	}
}; // class Format< double >

} /* namespace cfgfile */

#endif // CFGFILE__FORMAT_HPP__INCLUDED
