
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
#include "string_format.hpp"

// C++ include.
#include <memory>
#include <stack>

#if defined( CFGFILE_QT_SUPPORT ) && defined( CFGFILE_XML_SUPPORT )
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
template< typename Trait >
class parser_base_t {
public:
	explicit parser_base_t( tag_t< Trait > & tag )
		:	m_tag( tag )
	{
	}

	virtual ~parser_base_t()
	{
	}

	//! Do parsing.
	virtual void parse( const typename Trait::string_t & file_name ) = 0;

protected:

	void check_parser_state_after_parsing()
	{
		if( !m_stack.empty() )
			throw exception_t< Trait >(
				Trait::from_ascii( "Unexpected end of file. "
					"Still unfinished tag \"" ) + m_stack.top()->name() +
				Trait::from_ascii( "\"." ) );

		check_is_child_mandatory_tags_defined( m_tag, true );
	}

	void check_is_child_mandatory_tags_defined( const tag_t< Trait > & tag,
		bool first = false )
	{
		if( first )
		{
			if( tag.is_mandatory() && !tag.is_defined() )
				throw exception_t< Trait >(
					Trait::from_ascii( "Undefined mandatory tag: \"" ) +
					tag.name() + Trait::from_ascii( "\"." ) );

			for( tag_t< Trait > * t : tag.children() )
				check_is_child_mandatory_tags_defined( *t );
		}
		else if( tag.is_mandatory() && !tag.is_defined() )
		{
			throw exception_t< Trait >(
				Trait::from_ascii( "Undefined mandatory tag: \"" ) +
				tag.name() + Trait::from_ascii( "\"." ) );
		}

		if( tag.is_mandatory() )
		{
			for( tag_t< Trait > * t : tag.children() )
				check_is_child_mandatory_tags_defined( *t );
		}
	}


protected:
	//! Tag.
	tag_t< Trait > & m_tag;
	//! Stack of tags.
	std::stack< tag_t< Trait > * > m_stack;
}; // class parser_base_t


//
// parser_conffile_impl_t
//

//! Implementation of parser in cfgfile format.
template< typename Trait = string_trait_t >
class parser_conffile_impl_t final
	:	public parser_base_t< Trait >
{
public:
	parser_conffile_impl_t( tag_t< Trait > & tag,
		input_stream_t< Trait > & stream )
		:	parser_base_t< Trait >( tag )
		,	m_lex( stream )
	{
	}

	~parser_conffile_impl_t()
	{
	}

	//! Do parsing.
	void parse( const typename Trait::string_t & file_name )
	{
		if( !start_first_tag_parsing() )
			return;

		lexeme_t< Trait > lexeme = m_lex.next_lexeme();

		while( !lexeme.is_null() )
		{
			if( !m_stack.empty() )
			{
				if( lexeme.type() == lexeme_type_t::start )
					start_tag_parsing( *m_stack.top(),
						m_stack.top()->children() );
				else if( lexeme.type() == lexeme_type_t::string )
					m_stack.top()->on_string( parser_info_t< Trait >(
							file_name,
							m_lex.line_number(),
							m_lex.column_number() ),
						lexeme.value() );
				else if( lexeme.type() == lexeme_type_t::finish )
				{
					m_stack.top()->on_finish( parser_info_t< Trait >(
						file_name,
						m_lex.line_number(),
						m_lex.column_number() ) );
					m_stack.pop();
				}
			}
			else
				throw exception_t< Trait >(
					Trait::from_ascii( "Unexpected content. "
						"We've finished parsing, but we've got this: \"" ) +
					lexeme.value() + Trait::from_ascii( "\". " ) +
					Trait::from_ascii( "In file \"" ) + file_name +
					Trait::from_ascii( "\" on line " ) +
					Trait::to_string( m_lex.input_stream().line_number() ) +
					Trait::from_ascii( "." ) );


			lexeme = m_lex.next_lexeme();
		}

		check_parser_state_after_parsing();
	}

private:

	bool start_first_tag_parsing()
	{
		lexeme_t< Trait > lexeme = m_lex.next_lexeme();

		if( m_tag.is_mandatory() && lexeme.type() == lexeme_type_t::null )
			throw exception_t< Trait >( Trait::from_ascii( "Unexpected end of file. "
					"Undefined mandatory tag \"" ) + m_tag.name() +
				Trait::from_ascii( "\". In file \"" ) +
				m_lex.input_stream().file_name() +
				Trait::from_ascii( "\" on line " ) +
				Trait::to_string( m_lex.input_stream().line_number() ) +
				Trait::from_ascii( "." ) );
		else if( !m_tag.is_mandatory() && lexeme.type() == lexeme_type_t::null )
			return false;
		else if( lexeme.type() != lexeme_type_t::start )
			throw exception_t< Trait >(
				Trait::from_ascii( "Expected start curl brace, "
					"but we've got \"" ) + lexeme.value() +
				Trait::from_ascii( "\". In file \"" ) + m_lex.input_stream().file_name() +
				Trait::from_ascii( "\" on line " ) +
				Trait::to_string( m_lex.input_stream().line_number() ) +
				Trait::from_ascii( "." ) );

		lexeme = m_lex.next_lexeme();

		if( !start_tag_parsing( lexeme, m_tag ) )
			throw exception_t< Trait >(
				Trait::from_ascii( "Unexpected tag name. "
					"We expected \"" ) + m_tag.name() +
				Trait::from_ascii( "\", but we've got \"" ) + lexeme.value() +
				Trait::from_ascii( "\". In file \"" ) + m_lex.input_stream().file_name() +
				Trait::from_ascii( "\" on line " ) +
				Trait::to_string( m_lex.input_stream().line_number() ) +
				Trait::from_ascii( "." ) );

		return true;
	}

	bool start_tag_parsing( const lexeme_t< Trait > & lexeme,
		tag_t< Trait > & tag )
	{
		if( lexeme.type() == lexeme_type_t::start )
			throw exception_t< Trait >(
				Trait::from_ascii( "Unexpected start curl brace. "
					"We expected tag name, but we've got start curl brace. "
					"In file \"" ) + m_lex.input_stream().file_name() +
				Trait::from_ascii( "\" on line " ) +
				Trait::to_string( m_lex.input_stream().line_number() ) +
				Trait::from_ascii( "." ) );
		else if( lexeme.type() == lexeme_type_t::finish )
			throw exception_t< Trait >(
				Trait::from_ascii( "Unexpected finish curl brace. "
					"We expected tag name, but we've got finish curl brace. "
					"In file \"" ) + m_lex.input_stream().file_name() +
				Trait::from_ascii( "\" on line " ) +
				Trait::to_string( m_lex.input_stream().line_number() ) +
				Trait::from_ascii( "." ) );
		else if( lexeme.type() == lexeme_type_t::null )
			throw exception_t< Trait >( Trait::from_ascii( "Unexpected end of file. "
					"In file \"" ) + m_lex.input_stream().file_name() +
				Trait::from_ascii( "\" on line " ) +
				Trait::to_string( m_lex.input_stream().line_number() ) +
				Trait::from_ascii( "." ) );
		else if( tag.name() == lexeme.value() )
		{
			m_stack.push( &tag );

			tag.on_start( parser_info_t< Trait >(
				m_lex.input_stream().file_name(),
				m_lex.line_number(),
				m_lex.column_number() ) );

			return true;
		}

		return false;
	}

	void start_tag_parsing( const tag_t< Trait > & parent,
		const typename tag_t< Trait >::child_tags_list_t & list )
	{
		lexeme_t< Trait > lexeme = m_lex.next_lexeme();

		bool tag_found = false;

		for( tag_t< Trait > * tag : list )
		{
			if( start_tag_parsing( lexeme, *tag ) )
			{
				tag_found = true;

				break;
			}
		}

		if( !tag_found )
			throw exception_t< Trait >(
				Trait::from_ascii( "Unexpected tag name. "
					"We expected one child tag of tag \"" ) +
				parent.name() +
				Trait::from_ascii( "\", but we've got \"" ) + lexeme.value() +
				Trait::from_ascii( "\". In file \"" ) +
				m_lex.input_stream().file_name() +
				Trait::from_ascii( "\" on line " ) +
				Trait::to_string( m_lex.input_stream().line_number() ) +
				Trait::from_ascii( "." ) );
	}

private:
	//! Lex.
	lexical_analyzer_t< Trait > m_lex;
}; // class parser_conffile_impl_t

#if defined( CFGFILE_QT_SUPPORT ) && defined( CFGFILE_XML_SUPPORT )

//
// parser_dom_impl_t
//

//! Implementation of parser in XML format.
template< typename Trait >
class parser_dom_impl_t final
	:	public parser_base_t< Trait >
{
public:
	parser_dom_impl_t( tag_t< Trait > & tag, const QDomDocument & dom )
		:	parser_base_t< Trait >( tag )
		,	m_dom( dom )
	{
	}

	~parser_dom_impl_t()
	{
	}

	//! Do parsing.
	void parse( const typename Trait::string_t & file_name ) override
	{
		QDomElement element = m_dom.documentElement();

		if( element.isNull() && m_tag.is_mandatory() )
			throw exception_t< Trait >(
				Trait::from_ascii( "Unexpected end of file. "
					"Undefined mandatory tag \"" ) + m_tag.name() +
				Trait::from_ascii( "\". In file \"" ) + file_name +
				Trait::from_ascii( "\" on line " ) +
				Trait::to_string( element.lineNumber() ) +
				Trait::from_ascii( "." ) );

		if( !element.isNull() )
		{
			if( element.tagName() != m_tag.name() )
				throw exception_t< Trait >(
					Trait::from_ascii( "Unexpected tag name. "
						"We expected \"" ) + m_tag.name() +
					Trait::from_ascii( "\", but we've got \"" ) +
					Trait::string_t( element.tagName() ) +
					Trait::from_ascii( "\". In file \"" ) + file_name +
					Trait::from_ascii( "\" on line " ) +
					Trait::to_string( element.lineNumber() ) +
					Trait::from_ascii( "." ) );

			m_stack.push( &m_tag );

			m_tag.on_start( parser_info_t< Trait >( file_name,
				element.lineNumber(),
				element.columnNumber() ) );
		}

		parse_tag( element, file_name );

		if( !element.isNull() )
		{
			m_stack.top()->on_finish( parser_info_t< Trait >(
				file_name,
				element.lineNumber(),
				element.columnNumber() ) );

			m_stack.pop();
		}

		check_parser_state_after_parsing();
	}

private:
	//! Parse tag.
	void parse_tag( const QDomElement & e, const typename Trait::string_t & file_name )
	{
		for( QDomNode n = e.firstChild(); !n.isNull();
			n = n.nextSibling() )
		{
			QDomElement child = n.toElement();

			if( !child.isNull() )
			{
				tag_t< Trait > * tag = find_tag( child.tagName(),
					m_stack.top()->children() );

				if( !tag )
					throw exception_t< Trait >(
						Trait::from_ascii( "Unexpected tag name. "
							"We expected one child tag of tag \"" ) +
						m_stack.top()->name() +
						Trait::from_ascii( "\", but we've got \"" ) +
						Trait::string_t( child.tagName() ) +
						Trait::from_ascii( "\". In file \"" ) + file_name +
						Trait::from_ascii( "\" on line " ) +
						Trait::to_string( child.lineNumber() ) +
						Trait::from_ascii( "." ) );

				m_stack.push( tag );

				tag->on_start( parser_info_t< Trait >( file_name,
					child.lineNumber(),
					child.columnNumber() ) );

				const QDomNamedNodeMap attributes = child.attributes();

				for( int i = 1; i <= attributes.count(); ++i )
				{
					const QDomAttr attr = attributes.namedItem(
						QString( "a" ) + QString::number( i ) ).toAttr();

					if( !attr.isNull() )
						tag->on_string( parser_info_t< Trait >(
								file_name,
								attr.lineNumber(),
								attr.columnNumber() ),
							from_cfgfile_format< Trait >( attr.value()
								.prepend( const_t< Trait >::c_quotes ).append( const_t< Trait >::c_quotes ) ) );
				}

				parse_tag( child, file_name );

				tag->on_finish( parser_info_t< Trait >(
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
					m_stack.top()->on_string( parser_info_t< Trait >(
							file_name,
							text.lineNumber(),
							text.columnNumber() ),
						from_cfgfile_format< Trait >( text.data() ) );
				}
				else
					throw exception_t< Trait >(
						Trait::from_ascii( "Unexpected tag name. "
							"We expected one child tag of tag \"" ) +
						m_stack.top()->name() +
						Trait::from_ascii( "\", but we've got \"" ) +
						Trait::string_t( n.nodeName() ) +
						Trait::from_ascii( "\". In file \"" ) + file_name +
						Trait::from_ascii( "\" on line " ) +
						Trait::to_string( n.lineNumber() ) +
						Trait::from_ascii( "." ) );
			}
		}
	}

	//! Find tag.
	tag_t< Trait > * find_tag( const typename Trait::string_t & name,
		const typename tag_t< Trait >::child_tags_list_t & list )
	{
		for( tag_t< Trait > * tag : list )
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
template< typename Trait = string_trait_t >
class parser_t final {
public:
	parser_t( tag_t< Trait > & tag, input_stream_t< Trait > & stream )
		:	m_d( std::make_unique< details::parser_conffile_impl_t< Trait > >
				( tag, stream ) )
	{
	}

#if defined( CFGFILE_QT_SUPPORT ) && defined( CFGFILE_XML_SUPPORT )
	parser_t( tag_t< Trait > & tag, const QDomDocument & dom )
		:	m_d( std::make_unique< details::parser_dom_impl_t< Trait > >
				( tag, dom ) )
	{
	}
#endif

    /*!
        Parse input stream.
        \throw exception_t< Trait > on errors.
    */
	void parse( const typename Trait::string_t & file_name )
	{
		m_d->parse( file_name );
	}

private:
    DISABLE_COPY( parser_t )

	std::unique_ptr< details::parser_base_t< Trait > > m_d;
}; // class parser_t

} /* namespace cfgfile */

#endif // CFGFILE__PARSER_HPP__INCLUDED
