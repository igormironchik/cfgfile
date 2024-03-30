
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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
	out << "We've loaded the configuration from file:\n";
	out << "stringValue: " << cfg.m_stringValue << "\n";

	if( !cfg.m_listOfStringValues.empty() )
	{
		out << "listOfStringValues: ";

		for( const auto & str : cfg.m_listOfStringValues )
			out << str << " ";

		out << "\n";
	}

	out << "intValue: " << cfg.m_intValue << "\n";

	if( !cfg.m_vectorOfTags.empty() )
	{
		int i = 1;

		out << "vecOfTags:" << "\n";

		for( const auto & pair : cfg.m_vectorOfTags )
		{
			out << "Pair " << i << ":\n";
			out << "stringValue: " << pair.m_stringValue << "\n";
			out << "intValue: " << pair.m_intValue << "\n";
			++i;
		}
	}

	out << "\nAnd now we will save new configuration to file \"new.cfg\". "
		"Look at it!\n";

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

	out << "All have been done!\n";
}
