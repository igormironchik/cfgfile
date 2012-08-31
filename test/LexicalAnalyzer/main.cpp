
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
#include <QtConfFile/private/Lex>
#include <QtConfFile/private/InputStream>
#include <QtConfFile/Exceptions>


class TestLexicalAnalyzer
	:	public QObject
{
	Q_OBJECT

private slots:
	void test_quotedLexeme1()
	{
		QBuffer buffer;
		buffer.open( QBuffer::ReadWrite );
		buffer.write( "{firstTag \"lexeme\"}" );
		buffer.seek( 0 );

		QtConfFile::InputStream input( &buffer, QTextCodec::codecForLocale(),
			QLatin1String( "test_quotedLexeme1" ) );
		QtConfFile::LexicalAnalyzer analyzer( input );

		QtConfFile::LexicalAnalyzer::Lexeme lex1 = analyzer.nextLexeme();
		QVERIFY( lex1.type() == QtConfFile::LexicalAnalyzer::StartTagLexeme );

		QtConfFile::LexicalAnalyzer::Lexeme lex2 = analyzer.nextLexeme();
		QVERIFY( lex2.type() == QtConfFile::LexicalAnalyzer::StringLexeme );
		QCOMPARE( lex2.value(), QLatin1String( "firstTag" ) );

		QtConfFile::LexicalAnalyzer::Lexeme lex3 = analyzer.nextLexeme();
		QVERIFY( lex3.type() == QtConfFile::LexicalAnalyzer::StringLexeme );
		QCOMPARE( lex3.value(), QLatin1String( "lexeme" ) );

		QtConfFile::LexicalAnalyzer::Lexeme lex4 = analyzer.nextLexeme();
		QVERIFY( lex4.type() == QtConfFile::LexicalAnalyzer::FinishTagLexeme );

		QVERIFY( input.atEnd() == true );
	}

	void test_quotedLexeme2()
	{
		QBuffer buffer;
		buffer.open( QBuffer::ReadWrite );
		buffer.write( " \r\n\t {firstTag\"lexeme\"}" );
		buffer.seek( 0 );

		QtConfFile::InputStream input( &buffer, QTextCodec::codecForLocale(),
			QLatin1String( "test_quotedLexeme2" ) );
		QtConfFile::LexicalAnalyzer analyzer( input );

		QtConfFile::LexicalAnalyzer::Lexeme lex1 = analyzer.nextLexeme();
		QVERIFY( lex1.type() == QtConfFile::LexicalAnalyzer::StartTagLexeme );

		QtConfFile::LexicalAnalyzer::Lexeme lex2 = analyzer.nextLexeme();
		QVERIFY( lex2.type() == QtConfFile::LexicalAnalyzer::StringLexeme );
		QCOMPARE( lex2.value(), QLatin1String( "firstTag" ) );

		QtConfFile::LexicalAnalyzer::Lexeme lex3 = analyzer.nextLexeme();
		QVERIFY( lex3.type() == QtConfFile::LexicalAnalyzer::StringLexeme );
		QCOMPARE( lex3.value(), QLatin1String( "lexeme" ) );

		QtConfFile::LexicalAnalyzer::Lexeme lex4 = analyzer.nextLexeme();
		QVERIFY( lex4.type() == QtConfFile::LexicalAnalyzer::FinishTagLexeme );

		QVERIFY( input.atEnd() == true );
	}

	void test_backSlahSequence()
	{
		QBuffer buffer;
		buffer.open( QBuffer::ReadWrite );
		buffer.write( " \r\n\t {firstTag\"\\n\\r\\t\\\"\\\\lexeme\"}" );
		buffer.seek( 0 );

		QtConfFile::InputStream input( &buffer, QTextCodec::codecForLocale(),
			QLatin1String( "test_backSlahSequence" ) );
		QtConfFile::LexicalAnalyzer analyzer( input );

		QtConfFile::LexicalAnalyzer::Lexeme lex1 = analyzer.nextLexeme();
		QVERIFY( lex1.type() == QtConfFile::LexicalAnalyzer::StartTagLexeme );

		QtConfFile::LexicalAnalyzer::Lexeme lex2 = analyzer.nextLexeme();
		QVERIFY( lex2.type() == QtConfFile::LexicalAnalyzer::StringLexeme );
		QCOMPARE( lex2.value(), QLatin1String( "firstTag" ) );

		QtConfFile::LexicalAnalyzer::Lexeme lex3 = analyzer.nextLexeme();
		QVERIFY( lex3.type() == QtConfFile::LexicalAnalyzer::StringLexeme );
		QCOMPARE( lex3.value(), QLatin1String( "\n\r\t\"\\lexeme" ) );

		QtConfFile::LexicalAnalyzer::Lexeme lex4 = analyzer.nextLexeme();
		QVERIFY( lex4.type() == QtConfFile::LexicalAnalyzer::FinishTagLexeme );

		QVERIFY( input.atEnd() == true );
	}

	void test_wrongBackSlahSequence()
	{
		QBuffer buffer;
		buffer.open( QBuffer::ReadWrite );
		buffer.write( " \r\n\t {firstTag\"\\glexeme\"}" );
		buffer.seek( 0 );

		QtConfFile::InputStream input( &buffer, QTextCodec::codecForLocale(),
			QLatin1String( "test_wrongBackSlahSequence" ) );
		QtConfFile::LexicalAnalyzer analyzer( input );

		QtConfFile::LexicalAnalyzer::Lexeme lex1 = analyzer.nextLexeme();
		QVERIFY( lex1.type() == QtConfFile::LexicalAnalyzer::StartTagLexeme );

		QtConfFile::LexicalAnalyzer::Lexeme lex2 = analyzer.nextLexeme();
		QVERIFY( lex2.type() == QtConfFile::LexicalAnalyzer::StringLexeme );
		QCOMPARE( lex2.value(), QLatin1String( "firstTag" ) );

		try {
			QtConfFile::LexicalAnalyzer::Lexeme lex3 = analyzer.nextLexeme();
			QVERIFY( true == false );
		}
		catch( const QtConfFile::Exception & x )
		{
			QCOMPARE( x.whatAsQString(), QLatin1String( "Unrecognized back-slash sequence: \"\\g\". "
				"In file \"test_wrongBackSlahSequence\" on line 2." ) );
		}
	}

	void test_oneLineComment()
	{
		QBuffer buffer;
		buffer.open( QBuffer::ReadWrite );
		buffer.write( "{firstTag \"value1\"}\r\n" );
		buffer.write( "|| Comment\r\n" );
		buffer.write( "{secondTag \"value2\"}\r\n" );
		buffer.seek( 0 );

		QtConfFile::InputStream input( &buffer, QTextCodec::codecForLocale(),
			QLatin1String( "test_oneLineComment" ) );
		QtConfFile::LexicalAnalyzer analyzer( input );

		QtConfFile::LexicalAnalyzer::Lexeme lex1 = analyzer.nextLexeme();
		QVERIFY( lex1.type() == QtConfFile::LexicalAnalyzer::StartTagLexeme );

		QtConfFile::LexicalAnalyzer::Lexeme lex2 = analyzer.nextLexeme();
		QVERIFY( lex2.type() == QtConfFile::LexicalAnalyzer::StringLexeme );
		QCOMPARE( lex2.value(), QLatin1String( "firstTag" ) );

		QtConfFile::LexicalAnalyzer::Lexeme lex3 = analyzer.nextLexeme();
		QVERIFY( lex3.type() == QtConfFile::LexicalAnalyzer::StringLexeme );
		QCOMPARE( lex3.value(), QLatin1String( "value1" ) );

		QtConfFile::LexicalAnalyzer::Lexeme lex4 = analyzer.nextLexeme();
		QVERIFY( lex4.type() == QtConfFile::LexicalAnalyzer::FinishTagLexeme );

		QtConfFile::LexicalAnalyzer::Lexeme lex5 = analyzer.nextLexeme();
		QVERIFY( lex5.type() == QtConfFile::LexicalAnalyzer::StartTagLexeme );

		QtConfFile::LexicalAnalyzer::Lexeme lex6 = analyzer.nextLexeme();
		QVERIFY( lex6.type() == QtConfFile::LexicalAnalyzer::StringLexeme );
		QCOMPARE( lex6.value(), QLatin1String( "secondTag" ) );

		QtConfFile::LexicalAnalyzer::Lexeme lex7 = analyzer.nextLexeme();
		QVERIFY( lex7.type() == QtConfFile::LexicalAnalyzer::StringLexeme );
		QCOMPARE( lex7.value(), QLatin1String( "value2" ) );

		QtConfFile::LexicalAnalyzer::Lexeme lex8 = analyzer.nextLexeme();
		QVERIFY( lex8.type() == QtConfFile::LexicalAnalyzer::FinishTagLexeme );
	}

	void test_multiLineComment()
	{
		QBuffer buffer;
		buffer.open( QBuffer::ReadWrite );
		buffer.write( "{firstTag \"value1\"}\r\n" );
		buffer.write( "|#\r\n" );
		buffer.write( "  Comment\r\n" );
		buffer.write( "#|\r\n" );
		buffer.write( "{secondTag \"value2\"}\r\n" );
		buffer.seek( 0 );

		QtConfFile::InputStream input( &buffer, QTextCodec::codecForLocale(),
			QLatin1String( "test_multiLineComment" ) );
		QtConfFile::LexicalAnalyzer analyzer( input );

		QtConfFile::LexicalAnalyzer::Lexeme lex1 = analyzer.nextLexeme();
		QVERIFY( lex1.type() == QtConfFile::LexicalAnalyzer::StartTagLexeme );

		QtConfFile::LexicalAnalyzer::Lexeme lex2 = analyzer.nextLexeme();
		QVERIFY( lex2.type() == QtConfFile::LexicalAnalyzer::StringLexeme );
		QCOMPARE( lex2.value(), QLatin1String( "firstTag" ) );

		QtConfFile::LexicalAnalyzer::Lexeme lex3 = analyzer.nextLexeme();
		QVERIFY( lex3.type() == QtConfFile::LexicalAnalyzer::StringLexeme );
		QCOMPARE( lex3.value(), QLatin1String( "value1" ) );

		QtConfFile::LexicalAnalyzer::Lexeme lex4 = analyzer.nextLexeme();
		QVERIFY( lex4.type() == QtConfFile::LexicalAnalyzer::FinishTagLexeme );

		QtConfFile::LexicalAnalyzer::Lexeme lex5 = analyzer.nextLexeme();
		QVERIFY( lex5.type() == QtConfFile::LexicalAnalyzer::StartTagLexeme );

		QtConfFile::LexicalAnalyzer::Lexeme lex6 = analyzer.nextLexeme();
		QVERIFY( lex6.type() == QtConfFile::LexicalAnalyzer::StringLexeme );
		QCOMPARE( lex6.value(), QLatin1String( "secondTag" ) );

		QtConfFile::LexicalAnalyzer::Lexeme lex7 = analyzer.nextLexeme();
		QVERIFY( lex7.type() == QtConfFile::LexicalAnalyzer::StringLexeme );
		QCOMPARE( lex7.value(), QLatin1String( "value2" ) );

		QtConfFile::LexicalAnalyzer::Lexeme lex8 = analyzer.nextLexeme();
		QVERIFY( lex8.type() == QtConfFile::LexicalAnalyzer::FinishTagLexeme );
	}
}; // class TestLexicalAnalyzer

QTEST_MAIN( TestLexicalAnalyzer )

#include "main.moc"
