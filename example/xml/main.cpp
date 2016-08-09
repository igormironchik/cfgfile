
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2012-2016 Igor Mironchik

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

// QtConfFile include.
#include <QtConfFile/Utils>
#include <QtConfFile/Exceptions>

// Qt include.
#include <QtCore/QDebug>
#include <QtCore/QTextStream>


int main( int argc, char ** argv )
{
	Q_UNUSED( argc )
	Q_UNUSED( argv )

	TagConfiguration readTag;

	try {
		QtConfFile::readQtConfFile( readTag, QLatin1String( "example.cfg" ),
			QTextCodec::codecForName( "UTF-8" ) );
	}
	catch( const QtConfFile::Exception & x )
	{
		qDebug() << x.whatAsQString();

		return 1;
	}

	Configuration cfg = readTag.configuration();

	QTextStream out( stdout );
	out << "We've loaded the configuration from file:" << endl;
	out << "stringValue: " << cfg.m_stringValue << endl;

	if( !cfg.m_listOfStringValues.isEmpty() )
	{
		out << "listOfStringValues: ";

		foreach( QString str, cfg.m_listOfStringValues )
			out << str << " ";

		out << endl;
	}

	out << "intValue: " << cfg.m_intValue << endl;

	if( !cfg.m_vectorOfTags.isEmpty() )
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

	TagConfiguration writeTag( cfg );

	try {
		QtConfFile::writeQtConfFile( writeTag, QLatin1String( "new.cfg" ),
			QTextCodec::codecForName( "UTF-8" ),
			QtConfFile::FileFormat::XMLFormat );
	}
	catch( const QtConfFile::Exception & x )
	{
		qDebug() << x.whatAsQString();

		return 1;
	}

	out << "All have been done!" << endl;
}
