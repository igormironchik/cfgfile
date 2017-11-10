
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

#if defined( CFGFILE_QT_SUPPORT ) && defined( CFGFILE_XML_SUPPORT )
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
template< class T, class Trait = string_trait_t >
class tag_scalar_vector_t
	:	public tag_t< Trait >
{
public:
	//! Vector of values.
	typedef std::vector< T > values_vector_t;

	//! Construct tag.
	explicit tag_scalar_vector_t( const typename Trait::string_t & name,
		bool is_mandatory = false )
		:	tag_t< Trait >( name, is_mandatory )
		,	m_constraint( nullptr )
	{
	}

	//! Construct tag.
	tag_scalar_vector_t( tag_t< Trait > & owner,
		const typename Trait::string_t & name,
		bool is_mandatory = false )
		:	tag_t< Trait >( owner, name, is_mandatory )
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
				throw exception_t< Trait >(
					Trait::from_ascii( "Invalid value: \"" ) +
					typename Trait::string_t(
						format_t< T, Trait >::to_string( v ) ) +
					Trait::from_ascii( "\". Value must match to the "
						"constraint in tag \"" ) +
					this->name() + Trait::from_ascii( "\"." ) );
		}

		m_values.push_back( v );

		this->set_defined();
	}

	//! Set values.
	void
	set_values( const values_vector_t & v )
	{
		m_values = v;

		this->set_defined();
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
		if( this->is_defined() )
			receiver = m_values;
	}

	//! Set constraint for the tag's value.
	void
	set_constraint( constraint_t< T > * c )
	{
		m_constraint = c;
	}

	//! Print tag to the output.
	typename Trait::string_t print( int indent = 0 ) const override
	{
		typename Trait::string_t result;

		if( this->is_defined() )
		{
			result.append( typename Trait::string_t( indent,
				const_t< Trait >::c_tab ) );
			result.push_back( const_t< Trait >::c_begin_tag );
			result.append( this->name() );

			for( const T & v : m_values )
			{
				result.push_back( const_t< Trait >::c_space );

				typename Trait::string_t value =
					format_t< T, Trait >::to_string( v );

				value = to_cfgfile_format< Trait >( value );

				result.append( value );
			}

			if( !this->children().empty() )
			{
				result.push_back( const_t< Trait >::c_carriage_return );

				for( const tag_t< Trait > * tag : this->children() )
					result.append( tag->print( indent + 1 ) );

				result.append( typename Trait::string_t( indent,
					const_t< Trait >::c_tab ) );
			}

			result.push_back( const_t< Trait >::c_end_tag );
			result.push_back( const_t< Trait >::c_carriage_return );
		}

		return result;
	}

#if defined( CFGFILE_QT_SUPPORT ) && defined( CFGFILE_XML_SUPPORT )
	//! Print tag to the output.
	void print( QDomDocument & doc, QDomElement * parent = 0 ) const override
	{
		if( this->is_defined() )
		{
			QDomElement this_element = doc.createElement( this->name() );

			if( !parent )
				doc.appendChild( this_element );
			else
				parent->appendChild( this_element );

			typename values_vector_t::size_type i = 1;

			for( const T & v : m_values )
			{
				typename Trait::string_t value =
					format_t< T, Trait >::to_string( v );

				value = to_cfgfile_format< Trait >( value );

				QString tmp = value;

				if( tmp.startsWith( const_t< Trait >::c_quotes ) &&
					tmp.endsWith( const_t< Trait >::c_quotes ) )
						tmp = tmp.mid( 1, tmp.length() - 2 );

				this_element.setAttribute( QString( "a" ) + QString::number( i ),
					tmp );

				++i;
			}

			if( !this->children().empty() )
			{
				for( const tag_t< Trait > * tag : this->children() )
					tag->print( doc, &this_element );
			}
		}
	}
#endif

	//! Called when tag parsing finished.
	void on_finish( const parser_info_t< Trait > & info ) override
	{
		for( const tag_t< Trait > * tag : this->children() )
		{
			if( tag->is_mandatory() && !tag->is_defined() )
				throw exception_t< Trait >(
					Trait::from_ascii( "Undefined child mandatory tag: \"" ) +
					tag->name() +
					Trait::from_ascii( "\". Where parent is: \"" ) +
					this->name() + Trait::from_ascii( "\". In file \"" ) +
					info.file_name() + Trait::from_ascii( "\" on line " ) +
					Trait::to_string( info.line_number() ) +
					Trait::from_ascii( "." ) );
		}
	}

	//! Called when string found.
	void on_string( const parser_info_t< Trait > & info,
		const typename Trait::string_t & str ) override
	{
		if( this->is_any_child_defined() )
			throw exception_t< Trait >(
				Trait::from_ascii( "Value \"" ) + str +
				Trait::from_ascii( "\" for tag \"" ) + this->name() +
				Trait::from_ascii( "\" must be defined before any child "
					"tag. In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) +
				Trait::from_ascii( "." ) );

		const T value = format_t< T, Trait >::from_string( info, str );

		if( m_constraint )
		{
			if( !m_constraint->check( value ) )
				throw exception_t< Trait >(
					Trait::from_ascii( "Invalid value: \"" ) +
					str + Trait::from_ascii( "\". Value must match to the "
						"constraint in tag \"" ) +
					this->name() + Trait::from_ascii( "\". In file \"" ) +
					info.file_name() + Trait::from_ascii( "\" on line " ) +
					Trait::to_string( info.line_number() ) +
					Trait::from_ascii( "." ) );
		}

		m_values.push_back( value );

		this->set_defined();
	}

private:
	//! Value of the tag.
	values_vector_t m_values;
	//! Constraint.
	constraint_t< T > * m_constraint;
}; // class tag_scalar_vector_t

} /* namespace cfgfile */

#endif // CFGFILE__TAG_SCALAR_VECTOR_HPP__INCLUDED
