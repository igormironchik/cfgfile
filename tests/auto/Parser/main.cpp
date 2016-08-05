
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2012 Igor Mironchik

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

// Qt include.
#include <QtTest/QtTest>
#include <QtCore/QBuffer>
#include <QtCore/QTextStream>

// QtConfFile include.
#include <QtConfFile/private/InputStream>
#include <QtConfFile/private/Parser>
#include <QtConfFile/private/Tag>
#include <QtConfFile/Exceptions>


class ThirdTag
	:	public QtConfFile::Tag
{
public:
	ThirdTag( Tag & owner )
		:	QtConfFile::Tag( owner, QString( "thirdTag" ), false )
		,	m_started( false )
		,	m_finished( false )
		,	m_withString( false )
	{
	}

	bool isStarted() const
	{
		return m_started;
	}

	bool isFinished() const
	{
		return m_finished;
	}

	bool isWithString() const
	{
		return m_withString;
	}

	QString print( int indent = 0 ) const
	{
		Q_UNUSED( indent )

		return QString();
	}

	void print( QDomDocument & doc, QDomElement * parent = 0 ) const
	{
		Q_UNUSED( doc )
		Q_UNUSED( parent )
	}

protected:
	void onStart( const QtConfFile::ParserInfo & info )
	{
		Q_UNUSED( info )

		m_started = true;
	}

	void onFinish( const QtConfFile::ParserInfo & info )
	{
		Q_UNUSED( info )

		m_finished = true;
	}

	void onString( const QtConfFile::ParserInfo & info,
		const QString & str )
	{
		Q_UNUSED( info )
		Q_UNUSED( str )

		m_withString = true;
	}

private:
	bool m_started;
	bool m_finished;
	bool m_withString;
}; // class ThirdTag


class SecondTag
	:	public QtConfFile::Tag
{
public:
	SecondTag( Tag & owner )
		:	QtConfFile::Tag( owner, QString( "secondTag" ), false )
		,	m_started( false )
		,	m_finished( false )
		,	m_withString( false )
		,	m_child( *this )
	{
	}

	const ThirdTag & thirdTag() const
	{
		return m_child;
	}

	bool isStarted() const
	{
		return m_started;
	}

	bool isFinished() const
	{
		return m_finished;
	}

	bool isWithString() const
	{
		return m_withString;
	}

	QString print( int indent = 0 ) const
	{
		Q_UNUSED( indent )

		return QString();
	}

	void print( QDomDocument & doc, QDomElement * parent = 0 ) const
	{
		Q_UNUSED( doc )
		Q_UNUSED( parent )
	}

protected:
	void onStart( const QtConfFile::ParserInfo & info )
	{
		Q_UNUSED( info )

		m_started = true;
	}

	void onFinish( const QtConfFile::ParserInfo & info )
	{
		Q_UNUSED( info )

		m_finished = true;
	}

	void onString( const QtConfFile::ParserInfo & info,
		const QString & str )
	{
		Q_UNUSED( info )
		Q_UNUSED( str )

		m_withString = true;
	}

private:
	bool m_started;
	bool m_finished;
	bool m_withString;
	ThirdTag m_child;
}; // class SecondTag


class FirstTag
	:	public QtConfFile::Tag
{
public:
	FirstTag()
		:	QtConfFile::Tag( QString( "firstTag" ), false )
		,	m_started( false )
		,	m_finished( false )
		,	m_withString( false )
		,	m_child( *this )
	{
	}

	const SecondTag & secondTag() const
	{
		return m_child;
	}

	bool isStarted() const
	{
		return m_started;
	}

	bool isFinished() const
	{
		return m_finished;
	}

	bool isWithString() const
	{
		return m_withString;
	}

	QString print( int indent = 0 ) const
	{
		Q_UNUSED( indent )

		return QString();
	}

	void print( QDomDocument & doc, QDomElement * parent = 0 ) const
	{
		Q_UNUSED( doc )
		Q_UNUSED( parent )
	}

protected:
	void onStart( const QtConfFile::ParserInfo & info )
	{
		Q_UNUSED( info )

		m_started = true;
	}

	void onFinish( const QtConfFile::ParserInfo & info )
	{
		Q_UNUSED( info )

		m_finished = true;
	}

	void onString( const QtConfFile::ParserInfo & info,
		const QString & str )
	{
		Q_UNUSED( info )
		Q_UNUSED( str )

		m_withString = true;
	}

private:
	bool m_started;
	bool m_finished;
	bool m_withString;
	SecondTag m_child;
}; // class FirstTag


class TestParser
	:	public QObject
{
	Q_OBJECT

private slots:
	void test_configWithOneTag()
	{
		QBuffer buffer;
		buffer.open( QBuffer::ReadWrite );
		buffer.write( "{firstTag \"lexeme1\"}" );
		buffer.seek( 0 );

		QtConfFile::InputStream input( &buffer, QTextCodec::codecForLocale(),
			QLatin1String( "test_configWithOneTag" ) );

		FirstTag firstTag;

		QtConfFile::Parser parser( firstTag, input );

		try {
		parser.parse( QLatin1String( "test_configWithOneTag" ) );
		}
		catch( const QtConfFile::Exception & x )
		{
			qDebug() << x.whatAsQString();
		}

		firstTag.setDefined();

		QVERIFY( firstTag.isDefined() == true );
		QVERIFY( firstTag.isStarted() == true );
		QVERIFY( firstTag.isFinished() == true );
		QVERIFY( firstTag.isWithString() == true );

		QVERIFY( firstTag.secondTag().isDefined() == false );
		QVERIFY( firstTag.secondTag().isStarted() == false );
		QVERIFY( firstTag.secondTag().isFinished() == false );
		QVERIFY( firstTag.secondTag().isWithString() == false );

		QVERIFY( firstTag.secondTag().thirdTag().isDefined() == false );
		QVERIFY( firstTag.secondTag().thirdTag().isStarted() == false );
		QVERIFY( firstTag.secondTag().thirdTag().isFinished() == false );
		QVERIFY( firstTag.secondTag().thirdTag().isWithString() == false );
	} // test_configWithOneTag

	void test_configWithThreeTags()
	{
		QBuffer buffer;
		buffer.open( QBuffer::ReadWrite );
		buffer.write( "{firstTag \"lexeme1\"\r\n" );
		buffer.write( "\t{secondTag \"lexeme2\"\r\n" );
		buffer.write( "\t\t{thirdTag \"lexeme3\"}\r\n" );
		buffer.write( "}\r\n" );
		buffer.write( "}" );
		buffer.seek( 0 );

		QtConfFile::InputStream input( &buffer, QTextCodec::codecForLocale(),
			QLatin1String( "test_configWithThreeTags" ) );

		FirstTag firstTag;

		QtConfFile::Parser parser( firstTag, input );

		parser.parse( QLatin1String( "test_configWithThreeTags" ) );

		firstTag.setDefined();

		QVERIFY( firstTag.isDefined() == true );
		QVERIFY( firstTag.isStarted() == true );
		QVERIFY( firstTag.isFinished() == true );
		QVERIFY( firstTag.isWithString() == true );

		QVERIFY( firstTag.secondTag().isDefined() == false );
		QVERIFY( firstTag.secondTag().isStarted() == true );
		QVERIFY( firstTag.secondTag().isFinished() == true );
		QVERIFY( firstTag.secondTag().isWithString() == true );

		QVERIFY( firstTag.secondTag().thirdTag().isDefined() == false );
		QVERIFY( firstTag.secondTag().thirdTag().isStarted() == true );
		QVERIFY( firstTag.secondTag().thirdTag().isFinished() == true );
		QVERIFY( firstTag.secondTag().thirdTag().isWithString() == true );
	} // test_configWithThreeTags

	void test_unexpectedStartCurlBrace()
	{
		QBuffer buffer;
		buffer.open( QBuffer::ReadWrite );
		buffer.write( "{{firstTag \"lexeme1\"}" );
		buffer.seek( 0 );

		QtConfFile::InputStream input( &buffer, QTextCodec::codecForLocale(),
			QLatin1String( "test_unexpectedStartCurlBrace" ) );

		FirstTag firstTag;

		QtConfFile::Parser parser( firstTag, input );

		try {
			parser.parse( QLatin1String( "test_unexpectedStartCurlBrace" ) );
		}
		catch( QtConfFile::Exception & x )
		{
			QCOMPARE( x.whatAsQString(),
				QLatin1String( "Unexpected start curl brace. "
					"We expected tag name, but we've got start curl brace. "
					"In file \"test_unexpectedStartCurlBrace\" on line 1." ) );

			return;
		}

		QVERIFY( true == false );
	} // test_unexpectedStartCurlBrace

	void test_unexpectedTagName1()
	{
		QBuffer buffer;
		buffer.open( QBuffer::ReadWrite );
		buffer.write( "{secondTag \"lexeme1\"}" );
		buffer.seek( 0 );

		QtConfFile::InputStream input( &buffer, QTextCodec::codecForLocale(),
			QLatin1String( "test_unexpectedTagName1" ) );

		FirstTag firstTag;

		QtConfFile::Parser parser( firstTag, input );

		try {
			parser.parse( QLatin1String( "test_unexpectedTagName1" ) );
		}
		catch( QtConfFile::Exception & x )
		{
			QCOMPARE( x.whatAsQString(),
				QLatin1String( "Unexpected tag name. "
					"We expected \"firstTag\", but we've got \"secondTag\". "
					"In file \"test_unexpectedTagName1\" on line 1." ) );

			return;
		}

		QVERIFY( true == false );
	} // test_unexpectedTagName1

	void test_unexpectedTagName2()
	{
		QBuffer buffer;
		buffer.open( QBuffer::ReadWrite );
		buffer.write( "{firstTag \"lexeme1\"\r\n" );
		buffer.write( "\t{thirdTag \"lexeme2\"}\r\n" );
		buffer.write( "}" );
		buffer.seek( 0 );

		QtConfFile::InputStream input( &buffer, QTextCodec::codecForLocale(),
			QLatin1String( "test_unexpectedTagName2" ) );

		FirstTag firstTag;

		QtConfFile::Parser parser( firstTag, input );

		try {
			parser.parse( QLatin1String( "test_unexpectedTagName2" ) );
		}
		catch( QtConfFile::Exception & x )
		{
			QCOMPARE( x.whatAsQString(),
				QLatin1String( "Unexpected tag name. "
					"We expected one child tag of tag \"firstTag\", "
					"but we've got \"thirdTag\". "
					"In file \"test_unexpectedTagName2\" on line 2." ) );

			return;
		}

		QVERIFY( true == false );
	} // test_unexpectedTagName2

	void test_unexpectedEndOfFile()
	{
		QBuffer buffer;
		buffer.open( QBuffer::ReadWrite );
		buffer.write( "{firstTag \"lexeme1\"\r\n" );
		buffer.write( "\t{secondTag \"lexeme2\"}\r\n" );
		buffer.seek( 0 );

		QtConfFile::InputStream input( &buffer, QTextCodec::codecForLocale(),
			QLatin1String( "test_unexpectedEndOfFile" ) );

		FirstTag firstTag;

		QtConfFile::Parser parser( firstTag, input );

		try {
			parser.parse( QLatin1String( "test_unexpectedEndOfFile" ) );
		}
		catch( QtConfFile::Exception & x )
		{
			QCOMPARE( x.whatAsQString(),
				QLatin1String( "Unexpected end of file. "
					"Still unfinished tag \"firstTag\"." ) );

			return;
		}

		QVERIFY( true == false );
	} // test_unexpectedEndOfFile
}; // class TestParser

QTEST_MAIN( TestParser )

#include "main.moc"
