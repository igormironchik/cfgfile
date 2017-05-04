
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

// cfgfile include.
#include <cfgfile/Utils>
#include <cfgfile/Exceptions>
#include <cfgfile/private/Parser>
#include <cfgfile/private/InputStream>

// Qt include.
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <QDomDocument>
#include <QDomElement>

#include <QTextStream>


namespace cfgfile {

namespace /* anonymous */ {

//
// FileCloser
//

//! Automatically close file.
class FileCloser {
public:
	FileCloser( const QString & fileName, QIODevice::OpenMode mode )
		:	m_file( fileName )
	{
		if( !m_file.open( mode ) )
			throw Exception( QString( "Couldn't open file: \"%1\"" )
				.arg( fileName ) );
	}

	~FileCloser()
	{
		m_file.close();
	}

	//! \return File.
	QFile & file()
	{
		return m_file;
	}

private:
	QFile m_file;
}; // class FileCloser


//
// DetermineFormat
//

//! Determine format of the configuration file.
class DetermineFormat {
public:
	DetermineFormat( QIODevice * dev, QTextCodec * codec )
		:	m_stream( dev )
	{
		m_stream.setCodec( codec );
	}

	~DetermineFormat()
	{
	}

	//! Determine file's format.
	FileFormat determineFormat()
	{
		static const QChar xml = QLatin1Char( '<' );

		QChar ch = 0x00;

		while( !m_stream.atEnd() )
		{
			m_stream >> ch;

			if( ch.isSpace() )
				continue;

			if( ch == xml )
				return XMLFormat;
			else
				return cfgfileFormat;
		}

		return cfgfileFormat;
	}

private:
	//! Stream.
	QTextStream m_stream;
}; // class DetermineFormat

} /* namespace anonymous */


//
// readcfgfile
//

void
readcfgfile( Tag & tag, const QString & fileName, QTextCodec * codec )
{
	FileCloser file( fileName, QIODevice::ReadOnly | QIODevice::Text );

	FileFormat fmt = cfgfileFormat;

	{
		DetermineFormat fmtDeterminator( &file.file(), codec );

		fmt = fmtDeterminator.determineFormat();
	}

	file.file().seek( 0 );

	switch( fmt )
	{
		case cfgfileFormat :
		{
			InputStream stream( &file.file(), codec, fileName );

			Parser parser( tag, stream );

			parser.parse( fileName );
		}
			break;

		case XMLFormat :
		{
			QDomDocument doc;

			QString error;
			int line = 0;
			int column = 0;

			if( !doc.setContent( &file.file(), true, &error, &line, &column ) )
				throw Exception( QString( "Unable to parse XML "
					"from file: \"%1\". \"%2\" On line %3, column %4." )
						.arg( fileName )
						.arg( error )
						.arg( QString::number( line ) )
						.arg( QString::number( column ) ) );

			Parser parser( tag, doc );

			parser.parse( fileName );
		}
			break;

		default :
			break;
	}
}


//
// writecfgfile
//

void
writecfgfile( const Tag & tag, const QString & fileName, QTextCodec * codec,
	FileFormat fmt )
{
	FileCloser file( fileName,
		QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text );

	switch( fmt )
	{
		case cfgfileFormat :
		{
			QString content = tag.print();

			QTextStream stream( &file.file() );
			stream.setCodec( codec );

			stream << content;
		}
			break;

		case XMLFormat :
		{
			QDomDocument doc;

			tag.print( doc );

			QTextStream stream( &file.file() );
			stream.setCodec( codec );

			stream << doc.toString( 4 );
		}
			break;

		default :
			break;
	}
}

} /* namespace cfgfile */
