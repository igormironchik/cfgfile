
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

// Example include.
#include "cfg.hpp"

// Qt include.
#include <QDebug>
#include <QTextStream>
#include <QFile>


int main( int argc, char ** argv )
{
	Q_UNUSED( argc )
	Q_UNUSED( argv )

	Configuration cfg;

	QFile inFile( "example.cfg" );

	if( !inFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
	{
		qDebug() << "Can't read file";

		return 1;
	}

	try {
		QTextStream stream( &inFile );

		TagConfiguration readTag;

		cfgfile::read_cfgfile( readTag, stream, "example.cfg" );

		inFile.close();

		cfg = readTag.configuration();
	}
	catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & x )
	{
		inFile.close();

		qDebug() << x.desc();

		return 1;
	}

	QTextStream out( stdout );
	out << "We've loaded the configuration from file:" << endl;
	out << "stringValue: " << cfg.m_stringValue << endl;

	if( !cfg.m_listOfStringValues.empty() )
	{
		out << "listOfStringValues: ";

		foreach( QString str, cfg.m_listOfStringValues )
			out << str << " ";

		out << endl;
	}

	out << "intValue: " << cfg.m_intValue << endl;

	if( !cfg.m_vectorOfTags.empty() )
	{
		int i = 1;

		out << "vecOfTags:" << endl;

		foreach( Configuration::Pair pair, cfg.m_vectorOfTags )
		{
			out << "Pair " << i << ":" << endl;
			out << "stringValue: " << pair.m_stringValue << endl;
			out << "intValue: " << pair.m_intValue << endl;
			++i;
		}
	}

	out << endl << "And now we will save new configuration to file \"new.cfg\". "
		"Look at it!" << endl;

	QFile outFile( "new.cfg" );

	if( !outFile.open( QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text ) )
	{
		qDebug() << "Can't write file";

		return 1;
	}

	try {
		QTextStream stream( &outFile );

		TagConfiguration writeTag( cfg );

		cfgfile::write_cfgfile( writeTag, stream,
			cfgfile::file_format_t::xml_format );

		outFile.close();
	}
	catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & x )
	{
		outFile.close();

		qDebug() << x.desc();

		return 1;
	}

	out << "All have been done!" << endl;
}
