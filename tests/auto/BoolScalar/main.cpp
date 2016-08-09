
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

#include "cfg.hpp"

// QtConfFile include.
#include <QtConfFile/Utils>

// Qt include.
#include <QtTest/QtTest>


class BoolScalarTest
	:	public QObject
{
	Q_OBJECT

private:
	Configuration loadConfig( const QString & fileName )
	{
		TagConfiguration readTag;

		QtConfFile::readQtConfFile( readTag, fileName,
			QTextCodec::codecForName( "UTF-8" ) );

		return readTag.configuration();
	}

	void checkConfig( const Configuration & cfg )
	{
		QCOMPARE( cfg.m_withTrue, true );
		QCOMPARE( cfg.m_withFalse, false );
	}

private slots:
	void testAllIsOkWithTrueAndFalse()
	{
		try {
			Configuration cfg = loadConfig(
				QLatin1String( "all_is_ok_with_true_and_false.cfg" ) );

			checkConfig( cfg );
		}
		catch( const QtConfFile::Exception & x )
		{
			qDebug() << x.whatAsQString();

			QVERIFY( true == false );
		}
	} // testAllIsOkWithTrueAndFalse

	void testAllIsOkWithOnAndOff()
	{
		try {
			Configuration cfg = loadConfig(
				QLatin1String( "all_is_ok_with_on_and_off.cfg" ) );

			checkConfig( cfg );
		}
		catch( const QtConfFile::Exception & x )
		{
			qDebug() << x.whatAsQString();

			QVERIFY( true == false );
		}
	} // testAllIsOkWithOnAndOff

	void testAllIsOkWith1And0()
	{
		try {
			Configuration cfg = loadConfig(
				QLatin1String( "all_is_ok_with_1_and_0.cfg" ) );

			checkConfig( cfg );
		}
		catch( const QtConfFile::Exception & x )
		{
			qDebug() << x.whatAsQString();

			QVERIFY( true == false );
		}
	} // testAllIsOkWith1And0

	void testFalseInsteadOfTrue()
	{
		try {
			Configuration cfg = loadConfig(
				QLatin1String( "false_instead_of_true.cfg" ) );

			QCOMPARE( cfg.m_withTrue, false );
			QCOMPARE( cfg.m_withFalse, false );
		}
		catch( const QtConfFile::Exception & x )
		{
			qDebug() << x.whatAsQString();

			QVERIFY( true == false );
		}
	} // testFalseInsteadOfTrue

	void testInvalidValue()
	{
		try {
			loadConfig(
				QLatin1String( "invalid_value.cfg" ) );

			QVERIFY( true == false );
		}
		catch( const QtConfFile::Exception & x )
		{
			QCOMPARE( QLatin1String( "Invalid value: \"123\". "
				"In file \"invalid_value.cfg\" on line 2." ),
				x.whatAsQString() );
		}
	} // testInvalidValue
}; // class BoolScalarTest

QTEST_MAIN( BoolScalarTest )

#include "main.moc"
