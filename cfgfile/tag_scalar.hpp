
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

#ifndef CFGFILE__TAG_SCALAR_HPP__INCLUDED
#define CFGFILE__TAG_SCALAR_HPP__INCLUDED

//cfgfile include.
#include "tag.hpp"
#include "constraint.hpp"
#include "format.hpp"
#include "string_format.hpp"
#include "exceptions.hpp"
#include "const.hpp"

#if defined( CFGFILE_QSTRING_BUILD ) && defined( CFGFILE_XML_BUILD )
// Qt include.
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>
#endif


namespace cfgfile {

//
// tag_scalar_t
//

//! Tag with scalar value.
template< class T >
class tag_scalar_t final
	:	public tag_t
{
public:
	explicit tag_scalar_t( const string_t & name, bool is_mandatory = false )
		:	tag_t( name, is_mandatory )
		,	m_constraint( nullptr )
	{
	}

	tag_scalar_t( tag_t & owner, const string_t & name,
		bool is_mandatory = false )
		:	tag_t( owner, name, is_mandatory )
		,	m_constraint( nullptr )
	{
	}

	~tag_scalar_t()
	{
	}

	//! \return Value of the tag.
	const T &
	value() const
	{
		return m_value;
	}

	//! Set value.
	void
	set_value( const T & v )
	{
		if( m_constraint )
		{
			if( !m_constraint->check( v ) )
				throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
					format_t< T >::to_string( v ) +
					SL( "\". Value must match to the constraint in tag \"" ) +
					name() + SL( "\"." ) );
		}

		m_value = v;

		set_defined();
	}

	/*!
		Query optional value.

		If isDefined() is true then \a receiver
		will be initialized with value of the tag,
		otherwise nothing with \a receiver will happen.
	*/
	void
	query_opt_value( T & receiver )
	{
		if( is_defined() )
			receiver = m_value;
	}

	//! Set constraint for the tag's value.
	void
	set_constraint( constraint_t< T > * c )
	{
		m_constraint = c;
	}

	//! Print tag to the output.
	string_t print( int indent = 0 ) const override
	{
		string_t result;

		if( is_defined() )
		{
			result.push_back( string_t( indent, c_tab ) );

			result.push_back( c_begin_tag );
			result.push_back( name() );
			result.push_back( c_space );

			string_t value = format_t< T >::to_string( m_value );
			value = to_cfgfile_format( value );

			result.push_back( value );

			if( !children().isEmpty() )
			{
				result.push_back( c_carriage_return );

				for( const tag_t * tag : children() )
					result.push_back( tag->print( indent + 1 ) );

				result.push_back( string_t( indent, c_tab ) );
			}

			result.push_back( c_end_tag );
			result.push_back( c_carriage_return );
		}

		return result;
	}

#if defined( CFGFILE_QSTRING_BUILD ) && defined( CFGFILE_XML_BUILD )
	//! Print tag to the output.
	void print( QDomDocument & doc, QDomElement * parent = 0 ) const override
	{
		if( is_defined() )
		{
			QDomElement this_element = doc.createElement( name() );

			if( !parent )
				doc.appendChild( this_element );
			else
				parent->appendChild( this_element );

			QString value = format_t< T >::to_string( m_value );
			value = to_cfgfile_format( value );

			QDomText data = doc.createTextNode( value );

			this_element.appendChild( data );

			if( !children().empty() )
			{
				for( const tag_t * tag : children() )
					tag->print( doc, &this_element );
			}
		}
	}
#endif

	//! Called when tag parsing finished.
	void on_finish( const parser_info_t & info ) override
	{
		if( !is_defined() )
			throw exception_t( string_t( SL( "Undefined value of tag: \"" ) ) +
				name() + SL( "\". In file \"" ) + info.file_name() +
				SL( "\" on line " ) + info.line_number() + SL( "." ) );

		for( const tag_t * tag : children() )
		{
			if( tag->is_mandatory() && !tag->is_defined() )
				throw exception_t( string_t( SL( "Undefined child mandatory tag: \"" ) ) +
					tag->name() + SL( "\". Where parent is: \"" ) +
					name() + SL( "\". In file \"" ) + info.file_name() +
					SL( "\" on line " ) + info.line_number() + SL( "." ) );
		}
	}

	//! Called when string found.
	void on_string( const parser_info_t & info,
		const string_t & str ) override
	{
		if( !is_defined() )
		{
			if( is_any_child_defined() )
				throw exception_t( string_t( SL( "Value \"" ) ) + str +
					SL( "\" for tag \"" ) + name() +
					SL( "\" must be defined before any child tag. In file \"" ) +
					info.file_name() + SL( "\" on line " ) +
					info.line_number() + SL( "." ) );

			T value = format_t< T >::from_string( info, str );

			if( m_constraint )
			{
				if( !m_constraint->check( value ) )
					throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
						str + SL( "\". Value must match to the constraint in tag \"" ) +
						name() + SL( "\". In file \"" ) + info.file_name() +
						SL( "\" on line " ) + info.line_number() + SL( "." ) );
			}

			m_value = value;

			set_defined();
		}
		else
			throw exception_t( string_t( SL( "Value for the tag \"" ) ) +
				name() + SL( "\" already defined. In file \"" ) +
				info.file_name() + SL( "\" on line " ) +
				info.line_number() + SL( "." ) );
	}

private:
	//! Value of the tag.
	T m_value;
	//! Constraint.
	constraint_t< T > * m_constraint;
}; // class tag_scalar_t


//
// tag_scalar_t< bool >
//

//! Tag with bool value.
template<>
class tag_scalar_t< bool >
	:	public tag_t
{
public:
	explicit tag_scalar_t( const string_t & name, bool is_mandatory = false )
		:	tag_t( name, is_mandatory )
		,	m_value( false )
	{
	}

	tag_scalar_t( tag_t & owner, const string_t & name,
		bool is_mandatory = false )
		:	tag_t( owner, name, is_mandatory )
		,	m_value( false )
	{
	}

	~tag_scalar_t()
	{
	}

	//! \return Value of the tag.
	const bool &
	value() const
	{
		return m_value;
	}

	//! Set value.
	void
	set_value( const bool & v )
	{
		m_value = v;

		set_defined();
	}

	/*!
		Query optional value.

		If isDefined() is true then \a receiver
		will be initialized with value of the tag,
		otherwise nothing with \a receiver will happen.
	*/
	void
	query_opt_value( bool & receiver )
	{
		if( is_defined() )
			receiver = m_value;
	}

	//! Print tag to the output.
	string_t print( int indent = 0 ) const override
	{
		string_t result;

		if( is_defined() )
		{
			result.push_back( string_t( indent, c_tab ) );

			result.push_back( c_begin_tag );
			result.push_back( name() );
			result.push_back( c_space );

			string_t value = format_t< bool >::to_string( m_value );
			value = toc_fgfile_format( value );

			result.push_back( value );

			if( !children().empty() )
			{
				result.push_back( c_carriage_return );

				for( const tag_t * tag : children() )
					result.push_back( tag->print( indent + 1 ) );

				result.push_back( string_t( indent, c_tab ) );
			}

			result.push_back( c_end_tag );
			result.push_back( c_carriage_return );
		}

		return result;
	}

#if defined( CFGFILE_QSTRING_BUILD ) && defined( CFGFILE_XML_BUILD )
	//! Print tag to the output.
	void print( QDomDocument & doc, QDomElement * parent = 0 ) const override
	{
		if( is_defined() )
		{
			QDomElement this_element = doc.createElement( name() );

			if( !parent )
				doc.appendChild( this_element );
			else
				parent->appendChild( this_element );

			string_t value = format_t< bool >::to_string( m_value );
			value = to_cfgfile_format( value );

			QDomText data = doc.createTextNode( value );

			this_element.appendChild( data );

			if( !children().empty() )
			{
				for( const tag_t * tag : children() )
					tag->print( doc, &this_element );
			}
		}
	}
#endif

	//! Called when tag parsing finished.
	void on_finish( const parser_info_t & info ) override
	{
		if( !is_defined() )
			throw exception_t( string_t( SL( "Undefined value of tag: \"" ) ) +
				name() + SL( "\". In file \"" ) + info.file_name() +
				SL( "\" on line " ) + info.line_number() + SL( "." ) );

		for( const tag_t * tag : children() )
		{
			if( tag->is_mandatory() && !tag->is_defined() )
				throw exception_t( string_t( SL( "Undefined child mandatory tag: \"" ) ) +
					tag->name() + SL( "\". Where parent is: \"" ) +
					name() + SL( "\". In file \"" ) + info.file_name() +
					SL( "\" on line " ) + info.line_number() + SL( "." ) );
		}
	}

	//! Called when string found.
	void on_string( const parser_info_t & info,
		const string_t & str ) override
	{
		if( !is_defined() )
		{
			if( is_any_child_defined() )
				throw exception_t( string_t( SL( "Value \"" ) ) + str +
					SL( "\" for tag \"" ) + name() +
					SL( "\" must be defined before any child tag. In file \"" ) +
					info.file_name() + SL( "\" on line " ) +
					info.line_number() + SL( "." ) );

			m_value = format_t< bool >::from_string( info, str );

			set_defined();
		}
		else
			throw exception_t( string_t( SL( "Value for the tag \"" ) ) +
				name() + SL( "\" already defined. In file \"" ) +
				info.file_name() + SL( "\" on line " ) +
				info.line_number() + SL( "." ) );
	}

private:
	//! Value of the tag.
	bool m_value;
}; // class tag_scalar_t


//
// tag_scalar_t< string_t >
//

static const pos_t c_max_string_length = 80;

//! Tag with bool value.
template<>
class tag_scalar_t< string_t >
	:	public Tag
{
public:
	explicit tag_scalar_t( const string_t & name, bool is_mandatory = false )
		:	tag_t( name, is_mandatory )
		,	m_constraint( nullptr )
	{
	}

	tag_scalar_t( tag_t & owner, const string_t & name,
		bool is_mandatory = false )
		:	tag_t( owner, name, is_mandatory )
		,	m_constraint( nullptr )
	{
	}

	~tag_scalar_t()
	{
	}

	//! \return Value of the tag.
	const string_t &
	value() const
	{
		return m_value;
	}

	//! Set value.
	void
	set_value( const string_t & v )
	{
		if( m_constraint )
		{
			if( !m_constraint->check( v ) )
				throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
					format_t< string_t >::to_string( v ) +
					SL( "\". Value must match to the constraint in tag \"" ) +
					name() + SL( "\"." ) );
		}

		m_value = v;

		set_defined();
	}

	/*!
		Query optional value.

		If isDefined() is true then \a receiver
		will be initialized with value of the tag,
		otherwise nothing with \a receiver will happen.
	*/
	void
	query_opt_value( string_t & receiver )
	{
		if( is_defined() )
			receiver = m_value;
	}

	//! Set constraint for the tag's value.
	void
	set_constraint( constraint_t< string_t > * c )
	{
		m_constraint = c;
	}

	//! Print tag to the output.
	string_t print( int indent = 0 ) const override
	{
		string_t result;

		if( is_defined() )
		{
			result.push_back( string_t( indent, c_tab ) );

			result.push_back( c_begin_tag );
			result.push_back( name() );
			result.push_back( c_space );

			string_t value = format_t< string_t >::to_string( m_value );

			const pos_t sections = ( value.length() / c_max_string_length +
				( value.length() % c_max_string_length > 0 ? 1 : 0 ) );

			if( sections )
			{
				const string_t spaces = string_t( name().length() + 2,
					c_space );

				for( int i = 0; i < sections; ++i )
				{
					if( i > 0 )
					{
						result.push_back( c_carriage_return );

						result.push_back( string_t( indent, c_tab ) );

						result.push_back( spaces );
					}

					const string_t tmp = to_cfgfile_format(
						value.substr( i * c_max_string_length, c_max_string_length ) );

					result.push_back( tmp );
				}
			}
			else
			{
				result.push_back( c_quotes );
				result.push_back( c_quotes );
			}

			if( !children().empty() )
			{
				result.push_back( c_carriage_return );

				for( const tag_t * tag : children() )
					result.push_back( tag->print( indent + 1 ) );

				result.push_back( string_t( indent, c_tab ) );
			}

			result.push_back( c_end_tag );
			result.push_back( c_carriage_return );
		}

		return result;
	}

#if defined( CFGFILE_QSTRING_BUILD ) && defined( CFGFILE_XML_BUILD )
	//! Print tag to the output.
	void print( QDomDocument & doc, QDomElement * parent = 0 ) const override
	{
		if( is_defined() )
		{
			QDomElement this_element = doc.createElement( name() );

			if( !parent )
				doc.appendChild( this_element );
			else
				parent->appendChild( this_element );

			string_t value = format_t< string_t >::to_string( m_value );
			value = to_cfgfile_format( value );

			QDomText data = doc.createTextNode( value );

			this_element.appendChild( data );

			if( !children().empty() )
			{
				for( const tag_t * tag : children() )
					tag->print( doc, &this_element );
			}
		}
	}
#endif

	//! Called when tag parsing finished.
	void on_finish( const parser_info_t & info ) override
	{
		if( m_constraint )
		{
			if( !m_constraint->check( m_value ) )
				throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
					m_value + SL( "\". Value must match to the constraint in tag \"" ) +
					name() + SL( "\". In file \"" ) +
					info.file_name() + SL( "\" on line " ) +
					info.line_number() + SL( "." ) );
		}

		if( !is_defined() )
			throw exception_t( string_t( SL( "Undefined value of tag: \"" ) ) +
				name() + SL( "\". In file \"" ) +
				info.file_name() + SL( "\" on line " ) +
				info.line_number() + SL( "." ) );

		for( const tag_t * tag : children() )
		{
			if( tag->is_mandatory() && !tag->is_defined() )
				throw exception_t( string_t( SL( "Undefined child mandatory tag: \"" ) ) +
					tag->name() + SL( "\". Where parent is: \"" ) +
					name() + SL( "\". In file \"" ) +
					info.file_name() + SL( "\" on line " ) +
					info.line_number() + SL( "." ) );
		}
	}

	//! Called when string found.
	void on_string( const parser_info_t & info,
		const string_t & str ) override
	{
		if( is_any_child_defined() )
			throw exception_t( string_t( SL( "Value \"" ) ) + str +
				SL( "\" for tag \"" ) + name() +
				SL( "\" must be defined before any child tag. In file \"" ) +
				info.file_name() + SL( "\" on line " ) +
				info.line_number() + SL( "." ) );

		const string_t value = format_t< string_t >::from_string( info, str );

		m_value.push_back( value );

		set_defined();
	}

private:
	//! Value of the tag.
	string_t m_value;
	//! Constraint.
	constraint_t< string_t > * m_constraint;
}; // class tag_scalar_t

} /* namespace cfgfile */

#endif // CFGFILE__TAG_SCALAR_HPP__INCLUDED
