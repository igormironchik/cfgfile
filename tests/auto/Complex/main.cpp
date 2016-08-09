
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


class ComplexTest
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
		QCOMPARE( cfg.m_stringValue, QLatin1String( "string" ) );
		QVERIFY( cfg.m_listOfStringValues.size() == 3 );
		QCOMPARE( cfg.m_listOfStringValues.at( 0 ), QLatin1String( "str1" ) );
		QCOMPARE( cfg.m_listOfStringValues.at( 1 ), QLatin1String( "str2" ) );
		QCOMPARE( cfg.m_listOfStringValues.at( 2 ), QLatin1String( "str3" ) );
		QVERIFY( cfg.m_intValue == 100 );
		QVERIFY( cfg.m_vectorOfTags.size() == 2 );
		QCOMPARE( cfg.m_vectorOfTags.at( 0 ).m_stringValue,
			QLatin1String( "string1" ) );
		QVERIFY( cfg.m_vectorOfTags.at( 0 ).m_intValue == 100 );
		QCOMPARE( cfg.m_vectorOfTags.at( 1 ).m_stringValue,
			QLatin1String( "string2" ) );
		QVERIFY( cfg.m_vectorOfTags.at( 1 ).m_intValue == 200 );
	}

private slots:
	void testAllIsOk()
	{
		try {
			Configuration cfg = loadConfig(
				QLatin1String( "all_is_ok.cfg" ) );

			checkConfig( cfg );
		}
		catch( const QtConfFile::Exception & x )
		{
			qDebug() << x.whatAsQString();

			QVERIFY( true == false );
		}
	} // testAllIsOk

	void testAllIsOkWithComments()
	{
		try {
			Configuration cfg = loadConfig(
				QLatin1String( "all_is_ok_with_comments.cfg" ) );

			checkConfig( cfg );
		}
		catch( const QtConfFile::Exception & x )
		{
			qDebug() << x.whatAsQString();

			QVERIFY( true == false );
		}
	} // testAllIsOkWithComments

	void testIncosistencyToStringConstraint()
	{
		try {
			loadConfig(
				QLatin1String( "inconsistency_to_string_constraint.cfg" ) );

			QVERIFY( true == false );
		}
		catch( const QtConfFile::Exception & x )
		{
			QCOMPARE( QLatin1String( "Invalid value: \"str4\". Value must match "
				"to the constraint in tag \"listOfStringValues\". "
				"In file \"inconsistency_to_string_constraint.cfg\" "
				"on line 3." ), x.whatAsQString() );
		}
	} // testIncosistencyToStringConstraint

	void testIncosistencyToIntConstraint()
	{
		try {
			loadConfig(
				QLatin1String( "inconsistency_to_int_constraint.cfg" ) );

			QVERIFY( true == false );
		}
		catch( const QtConfFile::Exception & x )
		{
			QCOMPARE( QLatin1String( "Invalid value: \"200\". Value must match "
				"to the constraint in tag \"intValue\". "
				"In file \"inconsistency_to_int_constraint.cfg\" "
				"on line 4." ), x.whatAsQString() );
		}
	} // testIncosistencyToIntConstraint

	void testUndefinedChildMandatoryTag()
	{
		try {
			loadConfig(
				QLatin1String( "undefined_child_mandatory_tag.cfg" ) );

			QVERIFY( true == false );
		}
		catch( const QtConfFile::Exception & x )
		{
			QCOMPARE( QLatin1String( "Undefined child mandatory tag: \"stringValue\". "
				"Where parent is: \"vecOfTags\". "
				"In file \"undefined_child_mandatory_tag.cfg\" on line 8." ),
				x.whatAsQString() );
		}
	} // testUndefinedChildMandatoryTag

	void testUndefinedMandatoryTag()
	{
		try {
			loadConfig(
				QLatin1String( "undefined_mandatory_tag.cfg" ) );

			QVERIFY( true == false );
		}
		catch( const QtConfFile::Exception & x )
		{
			QCOMPARE( QLatin1String( "Undefined child mandatory tag: \"stringValue\". "
				"Where parent is: \"cfg\". "
				"In file \"undefined_mandatory_tag.cfg\" on line 14." ),
				x.whatAsQString() );
		}
	} // testUndefinedMandatoryTag

	void testEmptyFile()
	{
		try {
			loadConfig(
				QLatin1String( "empty_file.cfg" ) );

			QVERIFY( true == false );
		}
		catch( const QtConfFile::Exception & x )
		{
			QCOMPARE( QLatin1String( "Unexpected end of file. Undefined "
				"mandatory tag \"cfg\". In file \"empty_file.cfg\" on line 1." ),
				x.whatAsQString() );
		}
	} // testEmptyFile
}; // class ComplexTest

QTEST_MAIN( ComplexTest )

#include "main.moc"
