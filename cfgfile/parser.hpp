
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2017 Igor Mironchik

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

#ifndef CFGFILE__PARSER_HPP__INCLUDED
#define CFGFILE__PARSER_HPP__INCLUDED

// cfgfile include.
#include "types.hpp"
#include "input_stream.hpp"
#include "tag.hpp"
#include "exceptions.hpp"
#include "lex.hpp"
#include "parser_info.hpp"
#include "const.hpp"

// C++ include.
#include <memory>
#include <stack>

#if defined( CFGFILE_QSTRING_BUILD ) && defined( CFGFILE_XML_BUILD )
// Qt include.
#include <QDomDocument>
#include <QDomElement>
#endif


namespace cfgfile {

namespace details {

//
// parser_base_t
//

//! Base implementation of parser.
class parser_base_t {
public:
	explicit parser_base_t( tag_t & tag )
		:	m_tag( tag )
	{
	}

	virtual ~parser_base_t()
	{
	}

	//! Do parsing.
	virtual void parse( const string_t & file_name ) = 0;

protected:

	void check_parser_state_after_parsing()
	{
		if( !m_stack.empty() )
			throw exception_t( string_t( SL( "Unexpected end of file. "
				"Still unfinished tag \"" ) ) + m_stack.top()->name() +
				SL( "\"." ) );

		check_is_child_mandatory_tags_defined( m_tag, true );
	}

	void check_is_child_mandatory_tags_defined( const tag_t & tag,
		bool first = false )
	{
		if( first )
		{
			if( tag.is_mandatory() && !tag.is_defined() )
				throw exception_t( string_t( SL( "Undefined mandatory tag: \"" ) ) +
					tag.name() + SL( "\"." ) );

			for( tag_t * t : tag.children() )
				check_is_child_mandatory_tags_defined( *t );
		}
		else if( tag.is_mandatory() && !tag.is_defined() )
		{
			throw exception_t( string_t( SL( "Undefined mandatory tag: \"" ) ) +
				tag.name() + SL( "\"." ) );
		}

		if( tag.is_mandatory() )
		{
			for( tag_t * t : tag.children() )
				check_is_child_mandatory_tags_defined( *t );
		}
	}


protected:
	//! Tag.
	tag_t & m_tag;
	//! Stack of tags.
	std::stack< tag_t* > m_stack;
}; // class parser_base_t


//
// parser_conffile_impl_t
//

//! Implementation of parser in cfgfile format.
class parser_conffile_impl_t final
	:	public parser_base_t
{
public:
	parser_conffile_impl_t( tag_t & tag, input_stream_t & stream )
		:	parser_base_t( tag )
		,	m_lex( stream )
	{
	}

	~parser_conffile_impl_t()
	{
	}

	//! Do parsing.
	void parse( const string_t & file_name )
	{
		if( !start_first_tag_parsing() )
			return;

		lexeme_t lexeme = m_lex.next_lexeme();

		while( !lexeme.is_null() )
		{
			if( !m_stack.empty() )
			{
				if( lexeme.type() == lexeme_type_t::start )
					start_tag_parsing( *m_stack.top(),
						m_stack.top()->children() );
				else if( lexeme.type() == lexeme_type_t::string )
					m_stack.top()->on_string( parser_info_t(
							file_name,
							m_lex.line_number(),
							m_lex.column_number() ),
						lexeme.value() );
				else if( lexeme.type() == lexeme_type_t::finish )
				{
					m_stack.top()->on_finish( parser_info_t(
						file_name,
						m_lex.line_number(),
						m_lex.column_number() ) );
					m_stack.pop();
				}
			}
			else
				throw exception_t( string_t( SL( "Unexpected content. "
						"We've finished parsing, but we've got this: \"" ) ) +
					lexeme.value() + SL( "\". " ) +
					SL( "In file \"" ) + file_name +
					SL( "\" on line " ) +
					std::to_string( m_lex.input_stream().line_number() ) +
					SL( "." ) );


			lexeme = m_lex.next_lexeme();
		}

		check_parser_state_after_parsing();
	}

private:

	bool start_first_tag_parsing()
	{
		lexeme_t lexeme = m_lex.next_lexeme();

		if( m_tag.is_mandatory() && lexeme.type() == lexeme_type_t::null )
			throw exception_t( string_t( SL( "Unexpected end of file. "
					"Undefined mandatory tag \"" ) ) + m_tag.name() +
				SL( "\". In file \"" ) + m_lex.input_stream().file_name() +
				SL( "\" on line " ) +
				std::to_string( m_lex.input_stream().line_number() ) +
				SL( "." ) );
		else if( !m_tag.is_mandatory() && lexeme.type() == lexeme_type_t::null )
			return false;
		else if( lexeme.type() != lexeme_type_t::start )
			throw exception_t( string_t( SL( "Expected start curl brace, "
					"but we've got \"" ) ) + lexeme.value() +
				SL( "\". In file \"" ) + m_lex.input_stream().file_name() +
				SL( "\" on line " ) +
				std::to_string( m_lex.input_stream().line_number() ) +
				SL( "." ) );

		lexeme = m_lex.next_lexeme();

		if( !start_tag_parsing( lexeme, m_tag ) )
			throw exception_t( string_t( SL( "Unexpected tag name. "
					"We expected \"" ) ) + m_tag.name() +
				SL( "\", but we've got \"" ) + lexeme.value() +
				SL( "\". In file \"" ) + m_lex.input_stream().file_name() +
				SL( "\" on line " ) +
				std::to_string( m_lex.input_stream().line_number() ) +
				SL( "." ) );

		return true;
	}

	bool start_tag_parsing( const lexeme_t & lexeme, tag_t & tag )
	{
		if( lexeme.type() == lexeme_type_t::start )
			throw exception_t( string_t( SL( "Unexpected start curl brace. "
					"We expected tag name, but we've got start curl brace. "
					"In file \"" ) ) + m_lex.input_stream().file_name() +
				SL( "\" on line " ) +
				std::to_string( m_lex.input_stream().line_number() ) +
				SL( "." ) );
		else if( lexeme.type() == lexeme_type_t::finish )
			throw exception_t( string_t( SL( "Unexpected finish curl brace. "
					"We expected tag name, but we've got finish curl brace. "
					"In file \"" ) ) + m_lex.input_stream().file_name() +
				SL( "\" on line " ) +
				std::to_string( m_lex.input_stream().line_number() ) +
				SL( "." ) );
		else if( lexeme.type() == lexeme_type_t::null )
			throw exception_t( string_t( SL( "Unexpected end of file. "
					"In file \"" ) ) + m_lex.input_stream().file_name() +
				SL( "\" on line " ) +
				std::to_string( m_lex.input_stream().line_number() ) +
				SL( "." ) );
		else if( tag.name() == lexeme.value() )
		{
			m_stack.push( &tag );

			tag.on_start( parser_info_t(
				m_lex.input_stream().file_name(),
				m_lex.line_number(),
				m_lex.column_number() ) );

			return true;
		}

		return false;
	}

	void start_tag_parsing( const tag_t & parent,
		const tag_t::child_tags_list_t & list )
	{
		lexeme_t lexeme = m_lex.next_lexeme();

		bool tag_found = false;

		for( tag_t * tag : list )
		{
			if( start_tag_parsing( lexeme, *tag ) )
			{
				tag_found = true;

				break;
			}
		}

		if( !tag_found )
			throw exception_t( string_t( SL( "Unexpected tag name. "
					"We expected one child tag of tag \"" ) ) +
				parent.name() +
				SL( "\", but we've got \"" ) + lexeme.value() +
				SL( "\". In file \"" ) + m_lex.input_stream().file_name() +
				SL( "\" on line " ) +
				std::to_string( m_lex.input_stream().line_number() ) +
				SL( "." ) );
	}

private:
	//! Lex.
	lexical_analyzer_t m_lex;
}; // class parser_conffile_impl_t

#if defined( CFGFILE_QSTRING_BUILD ) && defined( CFGFILE_XML_BUILD )

//
// parser_dom_impl_t
//

//! Implementation of parser in XML format.
class parser_dom_impl_t final
	:	public parser_base_t
{
public:
	parser_dom_impl_t( tag_t & tag, const QDomDocument & dom )
		:	parser_base_t( tag )
		,	m_dom( dom )
	{
	}

	~parser_dom_impl_t()
	{
	}

	//! Do parsing.
	void parse( const string_t & file_name ) override
	{
		QDomElement element = m_dom.documentElement();

		if( element.isNull() && m_tag.is_mandatory() )
			throw exception_t( string_t( SL( "Unexpected end of file. "
					"Undefined mandatory tag \"" ) ) + m_tag.name() +
				SL( "\". In file \"" ) + file_name
				SL( "\" on line " ) + element.lineNumber() +
				SL( "." ) );

		if( !element.isNull() )
		{
			if( element.tagName() != m_tag.name() )
				throw exception_t( string_t( SL( "Unexpected tag name. "
						"We expected \"" ) ) + m_tag.name() +
					SL( "\", but we've got \"" ) + element.tagName() +
					SL( "\". In file \"" ) + file_name +
					SL( "\" on line " ) + element.lineNumber() +
					SL( "." ) );

			m_stack.push( &m_tag );

			m_tag.on_start( parser_info_t( file_name,
				element.lineNumber(),
				element.columnNumber() ) );
		}

		parse_tag( element, file_name );

		if( !element.isNull() )
		{
			m_stack.top()->on_finish( parser_info_t(
				file_name,
				element.lineNumber(),
				element.columnNumber() ) );

			m_stack.pop();
		}

		check_parser_state_after_parsing();
	}

private:
	//! Parse tag.
	void parse_tag( const QDomElement & e, const string_t & file_name )
	{
		for( QDomNode n = e.firstChild(); !n.isNull();
			n = n.nextSibling() )
		{
			QDomElement child = n.toElement();

			if( !child.isNull() )
			{
				tag_t * tag = find_tag( child.tagName(),
					m_stack.top()->children() );

				if( !tag )
					throw exception_t( string_t( SL( "Unexpected tag name. "
							"We expected one child tag of tag \"" ) ) +
						m_stack.top()->name() +
						SL( "\", but we've got \"" ) + child.tagName() +
						SL( "\". In file \"" ) + file_name +
						SL( "\" on line " ) + child.lineNumber() +
						SL( "." ) );

				m_stack.push( tag );

				tag->on_start( parser_info_t( file_name,
					child.lineNumber(),
					child.columnNumber() ) );

				const QDomNamedNodeMap attributes = child.attributes();

				for( int i = 1; i <= attributes.count(); ++i )
				{
					const QDomAttr attr = attributes.namedItem(
						QString( "a" ) + QString::number( i ) ).toAttr();

					if( !attr.isNull() )
						tag->on_string( parser_info_t(
								file_name,
								attr.lineNumber(),
								attr.columnNumber() ),
							from_cfgfile_format( attr.value()
								.prepend( c_quote ).append( c_quote ) ) );
				}

				parse_tag( child, file_name );

				tag->on_finish( parser_info_t(
					file_name,
					child.lineNumber(),
					child.columnNumber() ) );

				m_stack.pop();
			}
			else
			{
				QDomText text = n.toText();

				if( !text.isNull() )
				{
					m_stack.top()->on_string( parser_info_t(
							file_name,
							text.lineNumber(),
							text.columnNumber() ),
						from_cfgfile_format( text.data() ) );
				}
				else
					throw exception_t( string_t( SL( "Unexpected tag name. "
							"We expected one child tag of tag \"" ) ) +
						m_stack.top()->name() +
						SL( "\", but we've got \"" ) + n.nodeName() +
						SL( "\". In file \"" ) + file_name +
						SL( "\" on line " ) + n.lineNumber() +
						SL( "." ) );
			}
		}
	}

	//! Find tag.
	tag_t * find_tag( const string_t & name,
		const tag_t::child_tags_list_t & list )
	{
		for( tag_t * tag : list )
		{
			if( tag->name() == name )
				return tag;
		}

		return 0;
	}

private:
	const QDomDocument & m_dom;
}; // class parser_dom_impl_t

#endif

} /* namespace details */


//
// parser_t
//

//! Parser of the configuration file.
class parser_t final {
public:
	parser_t( tag_t & tag, input_stream_t & stream )
		:	m_d( std::make_unique< details::parser_conffile_impl_t >
				( tag, stream ) )
	{
	}

#if defined( CFGFILE_QSTRING_BUILD ) && defined( CFGFILE_XML_BUILD )
	parser_t( tag_t & tag, const QDomDocument & dom );
		:	m_d( std::make_unique< details::parser_dom_impl_t > ( tag, dom ) )
	{
	}
#endif

    /*!
        Parse input stream.
        \throw exception_t on errors.
    */
	void parse( const string_t & file_name )
	{
		m_d->parse( file_name );
	}

private:
    DISABLE_COPY( parser_t )

	std::unique_ptr< details::parser_base_t > m_d;
}; // class parser_t

} /* namespace cfgfile */

#endif // CFGFILE__PARSER_HPP__INCLUDED
