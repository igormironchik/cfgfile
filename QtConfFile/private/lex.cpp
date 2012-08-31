
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

// QtConfFile include.
#include <QtConfFile/private/Lex>
#include <QtConfFile/private/InputStream>
#include <QtConfFile/Exceptions>


namespace QtConfFile {

static const QChar c_beginTag = QChar( '{' );
static const QChar c_endTag = QChar( '}' );
static const QChar c_quotes = QChar( '"' );
static const QChar c_n = QChar( 'n' );
static const QChar c_t = QChar( 't' );
static const QChar c_r = QChar( 'r' );
static const QChar c_backSlash = QChar( '\\' );
static const QChar c_space = QChar( ' ' );
static const QChar c_tab = QChar( '\t' );
static const QChar c_carriageReturn = QChar( '\n' );
static const QChar c_lineFeed = QChar( '\r' );
static const QChar c_verticalBar = QChar( '|' );
static const QChar c_sharp = QChar( '#' );

//
// LexicalAnalyzer::LexicalAnalyzerPrivate
//

struct LexicalAnalyzer::LexicalAnalyzerPrivate {
	explicit LexicalAnalyzerPrivate( InputStream & stream )
		:	m_stream( stream )
	{
	}

	//! \return Is character a space character?
	bool isSpaceChar( QChar ch )
	{
		if( ch == c_space || ch == c_tab ||
			ch == c_carriageReturn || ch == c_lineFeed )
				return true;
		else
			return false;
	}

	//! Skip spaces in the stream.
	void skipSpaces()
	{
		QChar ch = m_stream.get();

		while( isSpaceChar( ch ) )
		{
			ch = m_stream.get();
		}

		m_stream.putBack( ch );
	}

	//! Process back-slash sequence.
	bool processBackSlash( QChar & ch )
	{
		ch = m_stream.get();

		if( ch == c_n )
			ch = c_carriageReturn;
		else if( ch == c_t )
			ch = c_tab;
		else if( ch == c_r )
			ch = c_lineFeed;
		else if( ch == c_quotes )
			ch = c_quotes;
		else if( ch == c_backSlash )
			ch = c_backSlash;
		else
			return false;

		return true;
	}

	//! Skip one-line comment.
	void skipOneLineComment()
	{
		QChar ch = m_stream.get();

		while( ( ch != c_carriageReturn && ch != c_lineFeed ) && !m_stream.atEnd() )
			ch = m_stream.get();

		skipSpaces();
	}

	//! Skip multi-line comment.
	void skipMultiLineComment()
	{
		QChar ch = m_stream.get();

		if( m_stream.atEnd() )
			return;

		QChar nextChar = m_stream.get();

		while( ( ch != c_sharp && nextChar != c_verticalBar ) && !m_stream.atEnd() )
		{
			ch = nextChar;

			nextChar = m_stream.get();
		}

		skipSpaces();
	}

	InputStream & m_stream;
}; // struct LexicalAnalyzer::LexicalAnalyzerPrivate


//
// LexicalAnalyzer
//


LexicalAnalyzer::LexicalAnalyzer( InputStream & stream )
	:	d( new LexicalAnalyzerPrivate( stream ) )
{
}

LexicalAnalyzer::~LexicalAnalyzer()
{
}

LexicalAnalyzer::Lexeme
LexicalAnalyzer::nextLexeme()
{
	if( d->m_stream.atEnd() )
		return Lexeme( NullLexeme, QString() );

	QString result;

	bool quotedLexeme = false;
	bool firstSymbol = true;

	d->skipSpaces();

	while( true )
	{
		QChar ch = d->m_stream.get();

		if( ch == c_quotes )
		{
			if( quotedLexeme )
				break;
			else if( firstSymbol )
				quotedLexeme = true;
			else
			{
				d->m_stream.putBack( ch );
				break;
			}
		}
		else if( ch == c_backSlash )
		{
			QChar newChar;

			if( !quotedLexeme )
				result.append( ch );
			else if( d->processBackSlash( newChar ) )
				result.append( newChar );
			else
				throw Exception( QString( "Unrecognized back-slash sequence: \"\\%1\". "
					"In file \"%2\" on line %3." ).arg( newChar )
						.arg( d->m_stream.fileName() )
						.arg( d->m_stream.lineNumber() ) );
		}
		else if( ch == c_beginTag )
		{
			if( result.isEmpty() )
				return Lexeme( StartTagLexeme, ch );
			else if( quotedLexeme )
				result.append( ch );
			else
			{
				d->m_stream.putBack( ch );
				break;
			}
		}
		else if( ch == c_endTag )
		{
			if( result.isEmpty() )
				return Lexeme( FinishTagLexeme, ch );
			else if( quotedLexeme )
				result.append( ch );
			else
			{
				d->m_stream.putBack( ch );
				break;
			}
		}
		else if( ch == c_space || ch == c_tab )
		{
			if( quotedLexeme )
				result.append( ch );
			else
				break;
		}
		else if( ch == c_carriageReturn || ch == c_lineFeed )
		{
			if( quotedLexeme )
				throw Exception( QString( "Unfinished quoted lexeme. "
					"New line detected. In file \"%1\" on line %2." )
						.arg( d->m_stream.fileName() )
						.arg( d->m_stream.lineNumber() ) );
			else
				break;
		}
		else if( ch == c_verticalBar )
		{
			if( quotedLexeme )
				result.append( ch );
			else
			{
				QChar nextChar = d->m_stream.get();

				if( nextChar == c_verticalBar )
					d->skipOneLineComment();
				else if( nextChar == c_sharp )
					d->skipMultiLineComment();
				else
				{
					result.append( ch );
					d->m_stream.putBack( nextChar );
				}
			}
		}
		else
			result.append( ch );

		if( d->m_stream.atEnd() )
		{
			if( quotedLexeme )
				throw Exception( QString( "Unfinished quoted lexeme. "
					"End of file riched. In file \"%1\" on line %2." )
						 .arg( d->m_stream.fileName() )
						 .arg( d->m_stream.lineNumber() ) );
			else
				break;
		}

		firstSymbol = false;
	}

	return Lexeme( StringLexeme, result );
}

} /* namespace QtConfFile */
