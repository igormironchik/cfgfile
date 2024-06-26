
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef CFGFILE__TYPES_HPP__INCLUDED
#define CFGFILE__TYPES_HPP__INCLUDED

#ifndef CFGFILE_DISABLE_STL
// C++ include.
#include <string>
#include <iostream>
#include <cwctype>
#include <cctype>
#include <vector>
#endif // CFGFILE_DISABLE_STL

#ifdef CFGFILE_QT_SUPPORT
// Qt include.
#include <QString>
#include <QTextStream>
#endif // CFGFILE_QT_SUPPORT


namespace cfgfile {

#ifndef CFGFILE_DISABLE_STL

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

	//! Type of the buffer.
	using buf_t = std::vector< char_t >;

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

	static inline pos_t size_of_file( istream_t & stream )
	{
		stream.seekg( 0, std::ios::end );

		const auto s = stream.tellg();

		stream.seekg( 0 );

		return s;
	}

	static inline void fill_buf( istream_t & stream, buf_t & buf, pos_t buf_size, pos_t & pos, pos_t size )
	{
		pos_t actual_size = ( size - pos < buf_size ? size - pos : buf_size );

		buf = buf_t( actual_size, 0x00 );

		if( buf.size() > 0 )
			stream.read( &buf[ 0 ], actual_size );

		std::size_t skip = 0;

		for( auto it = buf.crbegin(), last = buf.crend(); it != last; ++it )
		{
			if( *it == 0x00 )
				++skip;
			else
				break;
		}

		if( skip > 0 )
			buf.resize( actual_size - skip );

		pos += actual_size;
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

	//! Type of the buffer.
	using buf_t = std::vector< char_t >;

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

	static inline pos_t size_of_file( istream_t & stream )
	{
		stream.seekg( 0, std::ios::end );

		const auto s = stream.tellg();

		stream.seekg( 0 );

		return s;
	}

	static inline void fill_buf( istream_t & stream, buf_t & buf, pos_t buf_size, pos_t & pos, pos_t size )
	{
		pos_t actual_size = ( size - pos < buf_size ? size - pos : buf_size );

		buf = buf_t( actual_size, 0x00 );

		if( buf.size() > 0 )
			stream.read( &buf[ 0 ], actual_size );

		std::size_t skip = 0;

		for( auto it = buf.crbegin(), last = buf.crend(); it != last; ++it )
		{
			if( *it == 0x00 )
				++skip;
			else
				break;
		}

		if( skip > 0 )
			buf.resize( actual_size - skip );

		pos += actual_size;
	}
}; // struct string_trait_t

#endif // CFGFILE_DISABLE_STL


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

	//! Type of the buffer.
	using buf_t = QString;

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

	static inline pos_t size_of_file( istream_t & stream )
	{
		return 0;
	}

	static inline void fill_buf( istream_t & stream, buf_t & buf, pos_t buf_size, pos_t & pos, pos_t size )
	{
		buf = stream.read( buf_size );
		pos += buf.size();
	}
}; // struct qstring_trait_t

#endif // CFGFILE_QT_SUPPORT


#if defined(CFGFILE_DISABLE_STL) && defined(CFGFILE_QT_SUPPORT)
using string_trait_t = qstring_trait_t;
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
