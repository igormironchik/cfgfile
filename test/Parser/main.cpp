
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
#include <QtConfFile/Tag>
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

protected:
	void onStart()
	{
		m_started = true;
	}

	void onFinish()
	{
		m_finished = true;
	}

	void onString( const QString & str )
	{
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

protected:
	void onStart()
	{
		m_started = true;
	}

	void onFinish()
	{
		m_finished = true;
	}

	void onString( const QString & str )
	{
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
		:	QtConfFile::Tag( QString( "firstTag" ), true )
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

protected:
	void onStart()
	{
		m_started = true;
	}

	void onFinish()
	{
		m_finished = true;
	}

	void onString( const QString & str )
	{
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
			parser.parse();
		}
		catch( QtConfFile::Exception & x )
		{
			QCOMPARE( x.whatAsQString(),
				QLatin1String( "Undefined main tag: \"firstTag\". "
							   "In file \"test_configWithOneTag\"." ) );

			firstTag.setDefined();
		}

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
}; // class TestParser

QTEST_MAIN( TestParser )

#include "main.moc"
