
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
// Lexeme
//

Lexeme::Lexeme()
	:	m_type( NullLexeme )
{
}

Lexeme::Lexeme( LexemeType type, const QString & value )
    :	m_type( type )
    ,	m_value( value )
{
}

Lexeme &
Lexeme::operator = ( const Lexeme & other )
{
	if( this != &other )
	{
		m_type = other.type();
		m_value = other.value();
	}

	return *this;
}

LexemeType
Lexeme::type() const
{
    return m_type;
}

const QString &
Lexeme::value() const
{
    return m_value;
}

bool
Lexeme::isNull() const
{
    return ( m_type == NullLexeme );
}


//
// LexicalAnalyzer::LexicalAnalyzerPrivate
//

class LexicalAnalyzer::LexicalAnalyzerPrivate {
public:
	explicit LexicalAnalyzerPrivate( InputStream & stream )
		:	m_stream( stream )
		,	m_lineNumber( m_stream.lineNumber() )
		,	m_columnNumber( m_stream.columnNumber() )
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
		if( m_stream.atEnd() )
			return;

		QChar ch = m_stream.get();

		while( isSpaceChar( ch ) )
		{
			if( m_stream.atEnd() )
				return;

			ch = m_stream.get();
		}

		m_stream.putBack( ch );
	}

	//! Process back-slash sequence.
	bool processBackSlash( QChar & ch )
	{
		if( m_stream.atEnd() )
			throw Exception( QString( "Unexpected end of file. "
				"Unfinished back slash sequence. "
				"In file \"%1\" on line %2." )
					 .arg( m_stream.fileName() )
					 .arg( m_stream.lineNumber() ) );

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
	}

	//! Skip multi-line comment.
	void skipMultiLineComment()
	{
		QChar ch = m_stream.get();

		if( m_stream.atEnd() )
			return;

		QChar nextChar = m_stream.get();

		if( ch == c_sharp && nextChar == c_verticalBar )
			return;

		while( !m_stream.atEnd() )
		{
			ch = nextChar;

			nextChar = m_stream.get();

			if( ch == c_sharp && nextChar == c_verticalBar )
				break;
		}
	}

	InputStream & m_stream;
	qint64 m_lineNumber;
	qint64 m_columnNumber;
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

Lexeme
LexicalAnalyzer::nextLexeme()
{
	QString result;

	bool quotedLexeme = false;
	bool firstSymbol = true;
	bool skipComment = false;

	d->skipSpaces();

	d->m_lineNumber = d->m_stream.lineNumber();
	d->m_columnNumber = d->m_stream.columnNumber();

	if( d->m_stream.atEnd() )
		return Lexeme( NullLexeme, QString() );

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
				{
					skipComment = true;

					d->skipOneLineComment();

					if( firstSymbol )
						d->skipSpaces();
					else
						break;
				}
				else if( nextChar == c_sharp )
				{
					skipComment = true;

					d->skipMultiLineComment();

					if( firstSymbol )
						d->skipSpaces();
					else
						break;
				}
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
			else if( result.isEmpty() )
				return Lexeme( NullLexeme, QString() );
			else
				break;
		}

		if( !skipComment )
			firstSymbol = false;
		else
			skipComment = false;
	}

	return Lexeme( StringLexeme, result );
}

InputStream &
LexicalAnalyzer::inputStream()
{
    return d->m_stream;
}

qint64
LexicalAnalyzer::lineNumber() const
{
	return d->m_lineNumber;
}

qint64
LexicalAnalyzer::columnNumber() const
{
	return d->m_columnNumber;
}

} /* namespace QtConfFile */
