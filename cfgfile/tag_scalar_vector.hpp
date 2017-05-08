
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

#ifndef CFGFILE__TAG_SCALAR_VECTOR_HPP__INCLUDED
#define CFGFILE__TAG_SCALAR_VECTOR_HPP__INCLUDED

//cfgfile include.
#include "tag.hpp"
#include "constraint.hpp"
#include "format.hpp"
#include "string_format.hpp"
#include "types.hpp"
#include "exceptions.hpp"
#include "parser_info.hpp"

#if defined( CFGFILE_string_t_BUILD ) && defined( CFGFILE_XML_BUILD )
// Qt include.
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>
#endif

// C++ include.
#include <vector>


namespace cfgfile {

//
// tag_scalar_vector_t
//

//! Tag with multiple scalar values.
template< class T >
class tag_scalar_vector_t
	:	public tag_t
{
public:
	typedef std::vector< T > values_vector_t;

	explicit tag_scalar_vector_t( const string_t & name,
		bool is_mandatory = false )
		:	tag_t( name, is_mandatory )
		,	m_constraint( nullptr )
	{
	}

	tag_scalar_vector_t( tag_t & owner, const string_t & name,
		bool is_mandatory = false )
		:	tag_t( owner, name, is_mandatory )
		,	m_constraint( nullptr )
	{
	}

	~tag_scalar_vector_t()
	{
	}

	//! \return Amount of the values.
	typename values_vector_t::size_type
	size() const
	{
		return m_values.size();
	}

	//! \return Value with the given index.
	const T &
	at( typename values_vector_t::size_type index ) const
	{
		return m_values.at( index );
	}

	//! \return All values.
	const values_vector_t &
	values() const
	{
		return m_values;
	}

	/*!
		Set value.

		Repeatly adds value to the end of vector.
	*/
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

		m_values.push_back( v );

		set_defined();
	}

	//! Set values.
	void
	set_values( const values_vector_t & v )
	{
		m_values = v;

		set_defined();
	}

	/*!
		Query optional values.

		If isDefined() is true then \a receiver
		will be initialized with values of the tag,
		otherwise nothing with \a receiver will happen.
	*/
	void
	query_opt_values( values_vector_t & receiver )
	{
		if( is_defined() )
			receiver = m_values;
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
			result.append( string_t( indent, c_tab ) );
			result.push_back( c_begin_tag );
			result.append( name() );

			for( const T & v : m_values )
			{
				result.push_back( c_space );

				string_t value = format_t< T >::to_string( v );

				value = to_cfgfile_format( value );

				result.append( value );
			}

			if( !children().empty() )
			{
				result.push_back( c_carriage_return );

				for( const tag_t * tag : children() )
					result.append( tag->print( indent + 1 ) );

				result.append( string_t( indent, c_tab ) );
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

			values_vector_t::size_type i = 1;

			for( const T & v : m_values )
			{
				string_t value = format_t< T >::to_string( v );

				value = to_cfgfile_format( value );

				QString tmp = value;

				if( tmp.startsWith( c_quotes ) && tmp.endsWith( c_quotes ) )
					tmp = tmp.mid( 1, tmp.length() - 2 );

				this_element.setAttribute( QString( "a" ) + QString::number( i ),
					tmp );

				++i;
			}

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
		for( const tag_t * tag : children() )
		{
			if( tag->is_mandatory() && !tag->is_defined() )
				throw exception_t( string_t( SL( "Undefined child mandatory tag: \"" ) ) +
					tag->name() + SL( "\". Where parent is: \"" ) +
					name() + SL( "\". In file \"" ) +
					info.file_name() + SL( "\" on line " ) +
					pos_to_string( info.line_number() ) + SL( "." ) );
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
				pos_to_string( info.line_number() ) + SL( "." ) );

		const T value = format_t< T >::from_string( info, str );

		if( m_constraint )
		{
			if( !m_constraint->check( value ) )
				throw exception_t( string_t( SL( "Invalid value: \"" ) ) +
					str + SL( "\". Value must match to the constraint in tag \"" ) +
					name() + SL( "\". In file \"" ) +
					info.file_name() + SL( "\" on line " ) +
					pos_to_string( info.line_number() ) + SL( "." ) );
		}

		m_values.push_back( value );

		set_defined();
	}

private:
	//! Value of the tag.
	values_vector_t m_values;
	//! Constraint.
	constraint_t< T > * m_constraint;
}; // class tag_scalar_vector_t

} /* namespace cfgfile */

#endif // CFGFILE__TAG_SCALAR_VECTOR_HPP__INCLUDED
