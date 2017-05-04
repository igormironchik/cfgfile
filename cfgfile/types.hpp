
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

#ifndef CFGFILE__TYPES_HPP__INCLUDED
#define CFGFILE__TYPES_HPP__INCLUDED

#ifdef CFGFILE_WSTRING_BUILD

// C++ include.
#include <string>
#include <list>
#include <iostream>

#elif defined( CFGFILE_QSTRING_BUILD )

// Qt include.
#include <QString>
#include <QTextStream>

#else

// C++ include.
#include <string>
#include <list>
#include <iostream>

#endif


namespace cfgfile {

#ifdef CFGFILE_WSTRING_BUILD

//! String type.
using String = std::wstring;

//! Char type.
using Char = String::value_type;

//! Out stream type.
using OutStreamType = std::wostream;

#define SL(str) L##str

#elif defined( CFGFILE_QSTRING_BUILD )

//! Char type.
using Char = QChar;

//! Out stream type.
using OutStreamType = QTextStream;

class String {
public:
	using size_type = int;

	String()
	{
	}

	String( size_type size, Char ch )
		:	m_str( size, ch )
	{
	}

	String( const char * str )
		:	m_str( str )
	{
	}

	String( const String & other )
		:	m_str( other.m_str )
	{
	}

	String( const QString & other )
		:	m_str( other )
	{
	}


	String( const Char * unicode, size_type size = -1 )
		:	m_str( unicode, size )
	{
	}

	String( Char ch )
		:	m_str( ch )
	{
	}

	String( QLatin1String str )
		:	m_str( str )
	{
	}

	String( const QByteArray & ba )
		:	m_str( ba )
	{
	}

	String & operator = ( const String & other )
	{
		m_str = other.m_str;

		return *this;
	}

	~String()
	{
	}

	operator QString ()
	{
		return m_str;
	}

	operator QString () const
	{
		return m_str;
	}

	inline bool empty() const
	{
		return m_str.isEmpty();
	}

	static const int npos = -1;

	inline int find( Char ch ) const
	{
		return m_str.indexOf( ch );
	}

	inline int find( const String & str ) const
	{
		return m_str.indexOf( str.m_str );
	}

	QString::iterator begin()
	{
		return m_str.begin();
	}

	QString::iterator end()
	{
		return m_str.end();
	}

	QString::const_iterator begin() const
	{
		return m_str.begin();
	}

	QString::const_iterator end() const
	{
		return m_str.end();
	}

	QString::const_iterator cbegin() const
	{
		return m_str.begin();
	}

	QString::const_iterator cend() const
	{
		return m_str.end();
	}

	size_type length() const
	{
		return m_str.length();
	}

	String substr( size_type pos, size_type count = npos ) const
	{
		return m_str.mid( pos, count );
	}

	friend bool operator == ( const String & s1, const String & s2 )
	{
		return ( s1.m_str == s2.m_str );
	}

	friend String operator + ( const String & s1, const String & s2 )
	{
		return String( s1.m_str + s2.m_str );
	}

	friend String operator + ( const String & s1, const char * s2 )
	{
		return String( s1.m_str + s2 );
	}

	friend String operator + ( const char * s1, const String & s2 )
	{
		return String( s1 + s2.m_str );
	}

	friend String operator + ( const String & s1, const char ch )
	{
		return String( s1.m_str + ch );
	}

	friend String operator + ( const char ch, const String & s2 )
	{
		return String( ch + s2.m_str );
	}

	friend OutStreamType & operator << ( OutStreamType & to,
		const String & what )
	{
		to << what.m_str;

		return to;
	}

	const Char operator [] ( size_type pos ) const
	{
		return m_str[ pos ];
	}

	String & append( const String & other )
	{
		m_str.append( other.m_str );

		return *this;
	}

	String & append( size_type count, Char ch )
	{
		m_str.append( QString( count, ch ) );

		return *this;
	}

	void clear()
	{
		m_str.clear();
	}

private:
	//! Actual string.
	QString m_str;
}; // class String

#define SL(str) str

#else

//! String type.
using String = std::string;

//! Char type.
using Char = String::value_type;

//! Out stream type.
using OutStreamType = std::ostream;

#define SL(str) str

#endif

} /* namespace cfgfile */

#endif // CFGFILE__TYPES_HPP__INCLUDED
