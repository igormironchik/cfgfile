
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
using string_t = std::wstring;

//! Char type.
using char_t = string_t::value_type;

//! Out stream type.
using ostream_t = std::wostream;

//! In stream type.
using istream_t = std::wistream;

//! Type of pos in stream.
using pos_t = std::streamoff;

#define SL(str) L##str

static inline string_t pos_to_string( pos_t pos )
{
	return std::to_wstring( pos );
}

#elif defined( CFGFILE_QSTRING_BUILD )

//! Char type.
using char_t = QChar;

//! Out stream type.
using ostream_t = QTextStream;

//! In stream type.
using istream_t = QTextStream;

//! Type of pos in stream.
using pos_t = quint64;

class string_t {
public:
	using size_type = int;

	string_t()
	{
	}

	string_t( size_type size, char_t ch )
		:	m_str( size, ch )
	{
	}

	string_t( const char * str )
		:	m_str( str )
	{
	}

	string_t( const QString & other )
		:	m_str( other )
	{
	}


	string_t( const char_t * unicode, size_type size = -1 )
		:	m_str( unicode, size )
	{
	}

	string_t( char_t ch )
		:	m_str( ch )
	{
	}

	string_t( QLatin1String str )
		:	m_str( str )
	{
	}

	string_t( const QByteArray & ba )
		:	m_str( ba )
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

	inline int find( char_t ch ) const
	{
		return m_str.indexOf( ch );
	}

	inline int find( const string_t & str ) const
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

	string_t substr( size_type pos, size_type count = npos ) const
	{
		return m_str.mid( pos, count );
	}

	friend bool operator == ( const string_t & s1, const string_t & s2 )
	{
		return ( s1.m_str == s2.m_str );
	}

	friend string_t operator + ( const string_t & s1, const string_t & s2 )
	{
		return string_t( s1.m_str + s2.m_str );
	}

	friend string_t operator + ( const string_t & s1, const char * s2 )
	{
		return string_t( s1.m_str + s2 );
	}

	friend string_t operator + ( const char * s1, const string_t & s2 )
	{
		return string_t( s1 + s2.m_str );
	}

	friend string_t operator + ( const string_t & s1, const char ch )
	{
		return string_t( s1.m_str + ch );
	}

	friend string_t operator + ( const char ch, const string_t & s2 )
	{
		return string_t( ch + s2.m_str );
	}

	friend ostream_t & operator << ( ostream_t & to,
		const string_t & what )
	{
		to << what.m_str;

		return to;
	}

	const char_t operator [] ( size_type pos ) const
	{
		return m_str[ pos ];
	}

	string_t & append( const string_t & other )
	{
		m_str.append( other.m_str );

		return *this;
	}

	string_t & append( size_type count, char_t ch )
	{
		m_str.append( QString( count, ch ) );

		return *this;
	}

	void clear()
	{
		m_str.clear();
	}

	void push_back( char_t ch )
	{
		m_str.append( ch );
	}

private:
	//! Actual string.
	QString m_str;
}; // class string_t

#define SL(str) str

static inline string_t pos_to_string( pos_t pos )
{
	return QString::number( pos );
}

#else

//! String type.
using string_t = std::string;

//! Char type.
using char_t = string_t::value_type;

//! Input stream type.
using istream_t = std::istream;

//! Type of pos in stream.
using pos_t = std::streamoff;

//! Output stream type.
using ostream_t = std::ostream;

#define SL(str) str

static inline string_t pos_to_string( pos_t pos )
{
	return std::to_string( pos );
}

#endif


//
// DISABLE_COPY
//

//! Macro for disabling copy.
#define DISABLE_COPY( Class ) \
	Class( const Class & ) = delete; \
	Class & operator= ( const Class & ) = delete;

} /* namespace cfgfile */

#endif // CFGFILE__TYPES_HPP__INCLUDED
