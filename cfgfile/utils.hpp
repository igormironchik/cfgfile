
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

#ifndef CFGFILE__UTILS_HPP__INCLUDED
#define CFGFILE__UTILS_HPP__INCLUDED

// cfgfile include.
#include "types.hpp"
#include "tag.hpp"
#include "input_stream.hpp"
#include "parser.hpp"
#include "exceptions.hpp"

#if defined( CFGFILE_QT_SUPPORT ) && defined( CFGFILE_XML_SUPPORT )
// Qt include.
#include <QDomDocument>
#include <QDomElement>
#endif


namespace cfgfile {

//
// file_format_t
//

//! Format of the conf file.
enum class file_format_t {
	//! cfgfile format.
	cfgfile_format,
	//! XML format.
	xml_format
}; // enum FileFormat


namespace details {

//
// determine_format_t
//

//! Determine format of the configuration file.
template< typename Trait = string_trait_t >
class determine_format_t final {
public:
	explicit determine_format_t( typename Trait::istream_t & stream )
		:	m_stream( stream )
	{
	}

	//! Determine file's format.
	file_format_t format()
	{
		static const typename Trait::char_t xml = Trait::from_ascii( '<' );

		typename Trait::char_t ch = 0x00;

		while( !Trait::is_at_end( m_stream ) )
		{
			m_stream >> ch;

			if( Trait::is_space( ch ) )
				continue;

			if( ch == xml )
				return file_format_t::xml_format;
			else
				return file_format_t::cfgfile_format;
		}

		return file_format_t::cfgfile_format;
	}

private:
	//! Stream.
	typename Trait::istream_t & m_stream;
}; // class determine_format_t

} /* namespace details */


//
// read_cfgfile
//

//! Read cfgfile configuration file.
template< typename Trait = string_trait_t >
static inline void read_cfgfile(
	//! Configuration tag.
	tag_t< Trait > & tag,
	//! Stream.
	typename Trait::istream_t & stream,
	//! File name.
	const typename Trait::string_t & file_name )
{
	file_format_t fmt = file_format_t::cfgfile_format;

	{
		details::determine_format_t< Trait > d( stream );

		fmt = d.format();
	}

	Trait::to_begin( stream );

	switch( fmt )
	{
		case file_format_t::cfgfile_format :
		{
			input_stream_t< Trait > is( file_name, stream );

			parser_t< Trait > parser( tag, is );

			parser.parse( file_name );
		}
			break;

		case file_format_t::xml_format :
		{
#ifdef CFGFILE_QT_SUPPORT
#ifdef CFGFILE_XML_SUPPORT
			QDomDocument doc;

			QString error;
			int line = 0;
			int column = 0;

			const QString data = stream.readAll();

			if( !doc.setContent( data, true, &error, &line, &column ) )
				throw exception_t< Trait >( QString( "Unable to parse XML "
					"from file: \"%1\". \"%2\" On line %3, column %4." )
						.arg( file_name )
						.arg( error )
						.arg( QString::number( line ) )
						.arg( QString::number( column ) ) );

			parser_t< Trait > parser( tag, doc );

			parser.parse( file_name );
#else
			throw exception_t< Trait >(
				Trait::from_ascii( "To use XML format build cfgfile "
					"with CFGFILE_XML_SUPPORT" ) );
#endif // CFGFILE_XML_SUPPORT
#else
			throw exception_t< Trait >(
				Trait::from_ascii( "XML supported only with Qt. Parsing of file \"" ) +
				file_name + Trait::from_ascii( "\" failed." ) );
#endif // CFGFILE_QT_SUPPORT
		}
			break;

		default :
			break;
	}
}


//
// write_cfgfile
//

//! Write cfgfile configuration file.
template< typename Trait >
static inline void write_cfgfile(
	//! Configuration tag.
	const tag_t< Trait > & tag,
	//! Stream.
	typename Trait::ostream_t & stream,
	//! Format of the file.
	file_format_t fmt = file_format_t::cfgfile_format )
{
	switch( fmt )
	{
		case file_format_t::cfgfile_format :
		{
			const typename Trait::string_t content = tag.print();

			stream << content;
		}
			break;

		case file_format_t::xml_format :
		{
#ifdef CFGFILE_QT_SUPPORT
#ifdef CFGFILE_XML_SUPPORT
			QDomDocument doc;

			tag.print( doc );

			stream << doc.toString( 4 );
#else
			throw exception_t< Trait >(
				Trait::from_ascii( "To use XML format build cfgfile "
					"with CFGFILE_XML_SUPPORT" ) );
#endif // CFGFILE_XML_SUPPORT
#else
			throw exception_t< Trait >(
				Trait::from_ascii( "XML supported only with Qt." ) );
#endif // CFGFILE_QT_SUPPORT
		}
			break;

		default :
			break;
	}
}

} /* namespace cfgfile */

#endif // CFGFILE__UTILS_HPP__INCLUDED
