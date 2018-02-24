
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

// C++ include.
#include <string>
#include <iostream>
#include <cwctype>
#include <cctype>

#ifdef CFGFILE_QT_SUPPORT
// Qt include.
#include <QString>
#include <QTextStream>
#endif // CFGFILE_QT_SUPPORT


namespace cfgfile {

//
// wstring_trait_t
//

//! Trait for std::wstring support.
struct wstring_trait_t final {
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

	static inline string_t to_string( pos_t pos )
	{
		return std::to_wstring( pos );
	}

	static inline string_t from_ascii( const std::string & str )
	{
		std::wstring res;
		res.assign( str.cbegin(), str.cend() );

		return res;
	}

	static inline char_t from_ascii( char ch )
	{
		return (char_t) ch;
	}

	static inline void noskipws( istream_t & stream )
	{
		stream >> std::noskipws;
	}

	static inline bool is_at_end( istream_t & stream )
	{
		char_t tmp = 0;

		stream >> tmp;

		if( tmp )
		{
			stream.putback( tmp );

			return false;
		}
		else
			return true;
	}

	static inline bool is_space( char_t ch )
	{
		return ( std::iswspace( ch ) != 0 );
	}

	static inline void to_begin( istream_t & stream )
	{
		stream.seekg( 0 );
	}
}; // struct wstring_trait_t


//
// string_trait_t
//

//! Trait for std::string support.
struct string_trait_t final {
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

	static inline string_t to_string( pos_t pos )
	{
		return std::to_string( pos );
	}

	static inline string_t from_ascii( const std::string & str )
	{
		return str;
	}

	static inline char_t from_ascii( char ch )
	{
		return ch;
	}

	static inline void noskipws( istream_t & stream )
	{
		stream >> std::noskipws;
	}

	static inline bool is_at_end( istream_t & stream )
	{
		char_t tmp = 0;

		stream >> tmp;

		if( tmp )
		{
			stream.putback( tmp );

			return false;
		}
		else
			return true;
	}

	static inline bool is_space( char_t ch )
	{
		return ( std::isspace( (int) ch ) != 0 );
	}

	static inline void to_begin( istream_t & stream )
	{
		stream.seekg( 0 );
	}
}; // struct string_trait_t


#ifdef CFGFILE_QT_SUPPORT

//
// qstring_wrapper_t
//

//! QString wrapper.
class qstring_wrapper_t {
public:
	using size_type = int;

	qstring_wrapper_t()
	{
	}

	qstring_wrapper_t( size_type size, QChar ch )
		:	m_str( size, ch )
	{
	}

	qstring_wrapper_t( const char * str )
		:	m_str( str )
	{
	}

	qstring_wrapper_t( const QString & other )
		:	m_str( other )
	{
	}


	qstring_wrapper_t( const QChar * unicode, size_type size = -1 )
		:	m_str( unicode, size )
	{
	}

	qstring_wrapper_t( QChar ch )
		:	m_str( ch )
	{
	}

	qstring_wrapper_t( QLatin1String str )
		:	m_str( str )
	{
	}

	qstring_wrapper_t( const QByteArray & ba )
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

	inline int find( QChar ch ) const
	{
		return m_str.indexOf( ch );
	}

	inline int find( const qstring_wrapper_t & str ) const
	{
		return m_str.indexOf( str.m_str );
	}

	inline int rfind( const qstring_wrapper_t & str ) const
	{
		return m_str.lastIndexOf( str.m_str );
	}

	inline int rfind( QChar ch ) const
	{
		return m_str.lastIndexOf( ch );
	}

	qstring_wrapper_t & replace( size_type pos, size_type count,
		const qstring_wrapper_t & v )
	{
		m_str.replace( pos, count, v.m_str );

		return *this;
	}

	inline QString::iterator begin()
	{
		return m_str.begin();
	}

	inline QString::iterator end()
	{
		return m_str.end();
	}

	inline QString::const_iterator begin() const
	{
		return m_str.begin();
	}

	inline QString::const_iterator end() const
	{
		return m_str.end();
	}

	inline QString::const_iterator cbegin() const
	{
		return m_str.begin();
	}

	inline QString::const_iterator cend() const
	{
		return m_str.end();
	}

	inline size_type length() const
	{
		return m_str.length();
	}


	inline const QChar at( int position ) const
	{
		return m_str.at( position );
	}


	inline qstring_wrapper_t substr( size_type pos, size_type count = npos ) const
	{
		return m_str.mid( pos, count );
	}

	friend bool operator == ( const qstring_wrapper_t & s1,
		const qstring_wrapper_t & s2 )
	{
		return ( s1.m_str == s2.m_str );
	}

	friend qstring_wrapper_t operator + ( const qstring_wrapper_t & s1,
		const qstring_wrapper_t & s2 )
	{
		return qstring_wrapper_t( s1.m_str + s2.m_str );
	}

	friend qstring_wrapper_t operator + ( const qstring_wrapper_t & s1,
		const char * s2 )
	{
		return qstring_wrapper_t( s1.m_str + s2 );
	}

	friend qstring_wrapper_t operator + ( const char * s1,
		const qstring_wrapper_t & s2 )
	{
		return qstring_wrapper_t( s1 + s2.m_str );
	}

	friend qstring_wrapper_t operator + ( const qstring_wrapper_t & s1,
		const char ch )
	{
		return qstring_wrapper_t( s1.m_str + ch );
	}

	friend qstring_wrapper_t operator + ( const char ch,
		const qstring_wrapper_t & s2 )
	{
		return qstring_wrapper_t( ch + s2.m_str );
	}

	friend QTextStream & operator << ( QTextStream & to,
		const qstring_wrapper_t & what )
	{
		to << what.m_str;

		return to;
	}

	inline const QChar operator [] ( size_type pos ) const
	{
		return m_str[ pos ];
	}

	inline qstring_wrapper_t & append( const qstring_wrapper_t & other )
	{
		m_str.append( other.m_str );

		return *this;
	}

	inline qstring_wrapper_t & append( size_type count, QChar ch )
	{
		m_str.append( QString( count, ch ) );

		return *this;
	}

	inline void clear()
	{
		m_str.clear();
	}

	inline void push_back( QChar ch )
	{
		m_str.append( ch );
	}

private:
	//! Actual string.
	QString m_str;
}; // class qstring_wrapper_t


//
// qstring_trait_t
//

//! Trait for QString support.
struct qstring_trait_t final {
	//! String type.
	using string_t = qstring_wrapper_t;

	//! Char type.
	using char_t = QChar;

	//! Out stream type.
	using ostream_t = QTextStream;

	//! In stream type.
	using istream_t = QTextStream;

	//! Type of pos in stream.
	using pos_t = qint64;

	static inline string_t to_string( pos_t pos )
	{
		return QString::number( pos );
	}

	static inline string_t from_ascii( const std::string & str )
	{
		return qstring_wrapper_t( QString( str.c_str() ) );
	}

	static inline char_t from_ascii( char ch )
	{
		return QLatin1Char( ch );
	}

	static inline void noskipws( istream_t & )
	{
	}

	static inline bool is_at_end( istream_t & stream )
	{
		return stream.atEnd();
	}

	static inline bool is_space( char_t ch )
	{
		return ch.isSpace();
	}

	static inline void to_begin( istream_t & stream )
	{
		stream.seek( 0 );
	}
}; // struct qstring_trait_t

#endif // CFGFILE_QT_SUPPORT


//
// DISABLE_COPY
//

//! Macro for disabling copy.
#define DISABLE_COPY( Class ) \
	Class( const Class & ) = delete; \
	Class & operator= ( const Class & ) = delete;

} /* namespace cfgfile */

#endif // CFGFILE__TYPES_HPP__INCLUDED
