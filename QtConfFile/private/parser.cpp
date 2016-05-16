
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
#include <QtConfFile/private/Parser>
#include <QtConfFile/private/Lex>
#include <QtConfFile/private/InputStream>
#include <QtConfFile/private/Tag>
#include <QtConfFile/private/StringFormat>
#include <QtConfFile/Exceptions>

// Qt include.
#include <QtCore/QStack>

#include <QDomDocument>


namespace QtConfFile {

namespace /* anonymous */ {

//
// ParserBaseImpl
//

//! Base implementation of parser.
class ParserBaseImpl {
public:
	explicit ParserBaseImpl( Tag & tag )
		:	m_tag( tag )
	{
	}

	virtual ~ParserBaseImpl()
	{
	}

	//! Do parsing.
	virtual void parse( const QString & fileName ) = 0;

protected:

	void checkParserStateAfterParsing()
	{
		if( !m_stack.isEmpty() )
			throw Exception( QString( "Unexpected end of file. "
				"Still unfinished tag \"%1\"." )
					.arg( m_stack.top()->name() ) );

		checkIsChildMandatoryTagsDefined( m_tag, true );
	}

	void checkIsChildMandatoryTagsDefined( const Tag & tag, bool first = false )
	{
		if( first )
		{
			if( tag.isMandatory() && !tag.isDefined() )
				throw Exception( QString( "Undefined mandatory tag: \"%1\"." )
					.arg( tag.name() ) );

			foreach( Tag * t, tag.children() )
				checkIsChildMandatoryTagsDefined( *t );
		}
		else if( tag.isMandatory() && !tag.isDefined() )
		{
			throw Exception( QString( "Undefined mandatory tag: \"%1\"." )
				.arg( tag.name() ) );
		}

		if( tag.isMandatory() )
		{
			foreach( Tag * t, tag.children() )
				checkIsChildMandatoryTagsDefined( *t );
		}
	}


protected:
	Tag & m_tag;
	QStack< Tag* > m_stack;
}; // class ParserBaseImpl


//
// ParserQtConfImpl
//

//! Implementation of parser in QtConfFile format.
class ParserQtConfImpl
	:	public ParserBaseImpl
{
public:
	ParserQtConfImpl( Tag & tag, InputStream & stream )
		:	ParserBaseImpl( tag )
		,	m_lex( stream )
	{
	}

	~ParserQtConfImpl()
	{
	}

	//! Do parsing.
	void parse( const QString & fileName )
	{
		if( !startFirstTagParsing() )
			return;

		Lexeme lexeme = m_lex.nextLexeme();

		while( !lexeme.isNull() )
		{
			if( !m_stack.isEmpty() )
			{
				if( lexeme.type() == StartTagLexeme )
					startTagParsing( *m_stack.top(),
						m_stack.top()->children() );
				else if( lexeme.type() == StringLexeme )
					m_stack.top()->onString( ParserInfo(
						fileName,
						m_lex.lineNumber(),
						m_lex.columnNumber() ),
							lexeme.value() );
				else if( lexeme.type() == FinishTagLexeme )
				{
					m_stack.top()->onFinish( ParserInfo(
						fileName,
						m_lex.lineNumber(),
						m_lex.columnNumber() ) );
					m_stack.pop();
				}
			}
			else
				throw Exception( QString( "Unexpected content. "
					"We've finished parsing, but we've got this: \"%1\". "
					"In file \"%2\" on line %3." )
						.arg( lexeme.value() )
						.arg( fileName )
						.arg( m_lex.inputStream().lineNumber() ) );

			lexeme = m_lex.nextLexeme();
		}

		checkParserStateAfterParsing();
	}

private:

	bool startFirstTagParsing()
	{
		Lexeme lexeme = m_lex.nextLexeme();

		if( m_tag.isMandatory() && lexeme.type() == NullLexeme )
			throw Exception( QString( "Unexpected end of file. "
				"Undefined mandatory tag \"%1\". "
				"In file \"%2\" on line %3." )
					.arg( m_tag.name() )
					.arg( m_lex.inputStream().fileName() )
					.arg( m_lex.inputStream().lineNumber() ) );
		else if( !m_tag.isMandatory() && lexeme.type() == NullLexeme )
			return false;
		else if( lexeme.type() != StartTagLexeme )
			throw Exception( QString( "Expected start curl brace, "
				"but we've got \"%1\". In file \"%2\" on line %3." )
					.arg( lexeme.value() )
					.arg( m_lex.inputStream().fileName() )
					.arg( m_lex.inputStream().lineNumber() ) );

		lexeme = m_lex.nextLexeme();

		if( !startTagParsing( lexeme, m_tag ) )
			throw Exception( QString( "Unexpected tag name. "
				"We expected \"%1\", but we've got \"%2\". "
				"In file \"%3\" on line %4." )
					.arg( m_tag.name() )
					.arg( lexeme.value() )
					.arg( m_lex.inputStream().fileName() )
					.arg( m_lex.inputStream().lineNumber() ) );

		return true;
	}

	bool startTagParsing( const Lexeme & lexeme, Tag & tag )
	{
		if( lexeme.type() == StartTagLexeme )
			throw Exception( QString( "Unexpected start curl brace. "
				"We expected tag name, but we've got start curl brace. "
				"In file \"%1\" on line %2." )
					.arg( m_lex.inputStream().fileName() )
					.arg( m_lex.inputStream().lineNumber() ) );
		else if( lexeme.type() == FinishTagLexeme )
			throw Exception( QString( "Unexpected finish curl brace. "
				"We expected tag name, but we've got finish curl brace. "
				"In file \"%1\" on line %2." )
					.arg( m_lex.inputStream().fileName() )
					.arg( m_lex.inputStream().lineNumber() ) );
		else if( lexeme.type() == NullLexeme )
			throw Exception( QString( "Unexpected end of file. "
				"In file \"%1\" on line %2." )
					.arg( m_lex.inputStream().fileName() )
					.arg( m_lex.inputStream().lineNumber() ) );
		else if( tag.name() == lexeme.value() )
		{
			m_stack.push( &tag );

			tag.onStart( ParserInfo(
				m_lex.inputStream().fileName(),
				m_lex.lineNumber(),
				m_lex.columnNumber() ) );

			return true;
		}

		return false;
	}

	void startTagParsing( const Tag & parent,
		const Tag::ChildTagsList & list )
	{
		Lexeme lexeme = m_lex.nextLexeme();

		bool tagFound = false;

		foreach( Tag * tag, list )
		{
			if( startTagParsing( lexeme, *tag ) )
			{
				tagFound = true;
				break;
			}
		}

		if( !tagFound )
			throw Exception( QString( "Unexpected tag name. "
				"We expected one child tag of tag \"%1\", "
				"but we've got \"%2\". "
				"In file \"%3\" on line %4." )
					.arg( parent.name() )
					.arg( lexeme.value() )
					.arg( m_lex.inputStream().fileName() )
					.arg( m_lex.inputStream().lineNumber() ) );
	}

private:
	LexicalAnalyzer m_lex;
}; // class ParserQtConfImpl


//
// ParserDomImpl
//

//! Implementation of parser in XML format.
class ParserDomImpl
	:	public ParserBaseImpl
{
public:
	ParserDomImpl( Tag & tag, const QDomDocument & dom )
		:	ParserBaseImpl( tag )
		,	m_dom( dom )
	{
	}

	~ParserDomImpl()
	{
	}

	//! Do parsing.
	void parse( const QString & fileName )
	{
		QDomElement element = m_dom.documentElement();

		if( element.isNull() && m_tag.isMandatory() )
			throw Exception( QString( "Unexpected end of file. "
				"Undefined mandatory tag \"%1\". "
				"In file \"%2\" on line %3." )
					.arg( m_tag.name() )
					.arg( fileName )
					.arg( element.lineNumber() ) );

		if( !element.isNull() )
		{
			if( element.tagName() != m_tag.name() )
				throw Exception( QString( "Unexpected tag name. "
					"We expected \"%1\", but we've got \"%2\". "
					"In file \"%3\" on line %4." )
						.arg( m_tag.name() )
						.arg( element.tagName() )
						.arg( fileName )
						.arg( element.lineNumber() ) );

			m_stack.push( &m_tag );

			m_tag.onStart( ParserInfo( fileName,
				element.lineNumber(),
				element.columnNumber() ) );
		}

		parseTag( element, fileName );

		if( !element.isNull() )
		{
			m_stack.top()->onFinish( ParserInfo(
				fileName,
				element.lineNumber(),
				element.columnNumber() ) );

			m_stack.pop();
		}

		checkParserStateAfterParsing();
	}

private:
	//! Parse tag.
	void parseTag( const QDomElement & e, const QString & fileName )
	{
		for( QDomNode n = e.firstChild(); !n.isNull();
			n = n.nextSibling() )
		{
			QDomElement child = n.toElement();

			if( !child.isNull() )
			{
				Tag * tag = findTag( child.tagName(),
					m_stack.top()->children() );

				if( !tag )
					throw Exception( QString( "Unexpected tag name. "
						"We expected one child tag of tag \"%1\", "
						"but we've got \"%2\". "
						"In file \"%3\" on line %4." )
							.arg( m_stack.top()->name() )
							.arg( child.tagName() )
							.arg( fileName )
							.arg( child.lineNumber() ) );

				m_stack.push( tag );

				tag->onStart( ParserInfo( fileName,
					child.lineNumber(),
					child.columnNumber() ) );

				const QDomNamedNodeMap attributes = child.attributes();

				for( int i = 1; i <= attributes.count(); ++i )
				{
					const QDomAttr attr = attributes.namedItem(
						QString( "a" ) + QString::number( i ) ).toAttr();

					if( !attr.isNull() )
						tag->onString( ParserInfo(
								fileName,
								attr.lineNumber(),
								attr.columnNumber() ),
							fromQtConfFileFormat( attr.value() ) );
				}

				parseTag( child, fileName );

				tag->onFinish( ParserInfo(
					fileName,
					child.lineNumber(),
					child.columnNumber() ) );

				m_stack.pop();
			}
			else
			{
				QDomText text = n.toText();

				if( !text.isNull() )
				{
					m_stack.top()->onString( ParserInfo(
							fileName,
							text.lineNumber(),
							text.columnNumber() ),
						fromQtConfFileFormat( text.data() ) );
				}
				else
					throw Exception( QString( "Unexpected tag name. "
						"We expected one child tag of tag \"%1\", "
						"but we've got \"%2\". "
						"In file \"%3\" on line %4." )
							.arg( m_stack.top()->name() )
							.arg( n.nodeName() )
							.arg( fileName )
							.arg( n.lineNumber() ) );
			}
		}
	}

	//! Find tag.
	Tag * findTag( const QString & name, const Tag::ChildTagsList & list )
	{
		foreach( Tag * tag, list )
		{
			if( tag->name() == name )
				return tag;
		}

		return 0;
	}

private:
	const QDomDocument & m_dom;
}; // class ParserDomImpl

} /* namespace anonymous */


//
// Parser::ParserPrivate
//

class Parser::ParserPrivate {
public:
	ParserPrivate( Tag & tag, InputStream & stream )
		:   m_parser( new ParserQtConfImpl( tag, stream ) )
    {
    }

	ParserPrivate( Tag & tag, const QDomDocument & dom )
		:	m_parser( new ParserDomImpl( tag, dom ) )
	{
	}

	QScopedPointer< ParserBaseImpl > m_parser;
}; // class Parser::ParserPrivate


//
// Parser
//

Parser::Parser( Tag & tag, InputStream & stream )
    :   d( new ParserPrivate( tag, stream ) )
{
}

Parser::Parser( Tag & tag, const QDomDocument & dom )
	:	d( new ParserPrivate( tag, dom ) )
{
}

Parser::~Parser()
{
}

void
Parser::parse( const QString & fileName )
{
	d->m_parser->parse( fileName );
}

} /* namespace QtConfFile */
