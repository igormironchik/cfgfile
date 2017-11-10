
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

#if defined( CFGFILE_QT_SUPPORT ) && defined( CFGFILE_XML_SUPPORT )
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
template< class T, class Trait = string_trait_t >
class tag_scalar_t
	:	public tag_t< Trait >
{
public:
	//! Construct tag.
	explicit tag_scalar_t( const typename Trait::string_t & name,
		bool is_mandatory = false )
		:	tag_t< Trait >( name, is_mandatory )
		,	m_value( T() )
		,	m_constraint( nullptr )
	{
	}

	//! Construct tag.
	tag_scalar_t( tag_t< Trait > & owner, const typename Trait::string_t & name,
		bool is_mandatory = false )
		:	tag_t< Trait >( owner, name, is_mandatory )
		,	m_value( T() )
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
				throw exception_t< Trait >(
					Trait::from_ascii( "Invalid value: \"" ) +
					typename Trait::string_t(
						format_t< T, Trait >::to_string( v ) ) +
					Trait::from_ascii( "\". Value must match to "
						"the constraint in tag \"" ) +
					this->name() + Trait::from_ascii( "\"." ) );
		}

		m_value = v;

		this->set_defined();
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
		if( this->is_defined() )
			receiver = m_value;
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
			result.push_back( const_t< Trait >::c_space );

			typename Trait::string_t value =
				format_t< T, Trait >::to_string( m_value );
			value = to_cfgfile_format< Trait >( value );

			result.append( value );

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

			QString value = format_t< T, Trait >::to_string( m_value );
			value = to_cfgfile_format< Trait >( value );

			QDomText data = doc.createTextNode( value );

			this_element.appendChild( data );

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
		if( !this->is_defined_member_value() )
			throw exception_t< Trait >(
				Trait::from_ascii( "Undefined value of tag: \"" ) +
				this->name() + Trait::from_ascii( "\". In file \"" ) +
				info.file_name() +
				Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) +
				Trait::from_ascii( "." ) );

		for( const tag_t< Trait > * tag : this->children() )
		{
			if( tag->is_mandatory() && !tag->is_defined() )
				throw exception_t< Trait >(
					Trait::from_ascii( "Undefined child mandatory tag: \"" ) +
					tag->name() + Trait::from_ascii( "\". Where parent is: \"" ) +
					this->name() + Trait::from_ascii( "\". In file \"" ) +
					info.file_name() +
					Trait::from_ascii( "\" on line " ) +
					Trait::to_string( info.line_number() ) +
					Trait::from_ascii( "." ) );
		}
	}

	//! Called when string found.
	void on_string( const parser_info_t< Trait > & info,
		const typename Trait::string_t & str ) override
	{
		if( !this->is_defined_member_value() )
		{
			if( this->is_any_child_defined() )
				throw exception_t< Trait >(
					Trait::from_ascii( "Value \"" ) + str +
					Trait::from_ascii( "\" for tag \"" ) + this->name() +
					Trait::from_ascii( "\" must be defined before any "
						"child tag. In file \"" ) +
					info.file_name() + Trait::from_ascii( "\" on line " ) +
					Trait::to_string( info.line_number() ) +
					Trait::from_ascii( "." ) );

			T value = format_t< T, Trait >::from_string( info, str );

			if( m_constraint )
			{
				if( !m_constraint->check( value ) )
					throw exception_t< Trait >(
						Trait::from_ascii( "Invalid value: \"" ) +
						str +
						Trait::from_ascii( "\". Value must match to the "
							"constraint in tag \"" ) +
						this->name() + Trait::from_ascii( "\". In file \"" ) +
						info.file_name() +
						Trait::from_ascii( "\" on line " ) +
						Trait::to_string( info.line_number() ) +
						Trait::from_ascii( "." ) );
			}

			m_value = value;

			this->set_defined();
		}
		else
			throw exception_t< Trait >(
				Trait::from_ascii( "Value for the tag \"" ) +
				this->name() +
				Trait::from_ascii( "\" already defined. In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) +
				Trait::from_ascii( "." ) );
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
template< typename Trait >
class tag_scalar_t< bool, Trait >
	:	public tag_t< Trait >
{
public:
	//! Construct tag.
	explicit tag_scalar_t( const typename Trait::string_t & name,
		bool is_mandatory = false )
		:	tag_t< Trait >( name, is_mandatory )
		,	m_value( false )
	{
	}

	//! Construct tag.
	tag_scalar_t( tag_t< Trait > & owner, const typename Trait::string_t & name,
		bool is_mandatory = false )
		:	tag_t< Trait >( owner, name, is_mandatory )
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

		this->set_defined();
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
		if( this->is_defined() )
			receiver = m_value;
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
			result.push_back( const_t< Trait >::c_space );

			typename Trait::string_t value =
				format_t< bool, Trait >::to_string( m_value );
			value = to_cfgfile_format< Trait >( value );

			result.append( value );

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

			typename Trait::string_t value =
				format_t< bool, Trait >::to_string( m_value );
			value = to_cfgfile_format< Trait >( value );

			QDomText data = doc.createTextNode( value );

			this_element.appendChild( data );

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
		if( !this->is_defined_member_value() )
			throw exception_t< Trait >(
				Trait::from_ascii( "Undefined value of tag: \"" ) +
				this->name() + Trait::from_ascii( "\". In file \"" ) +
				info.file_name() +
				Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) +
				Trait::from_ascii( "." ) );

		for( const tag_t< Trait > * tag : this->children() )
		{
			if( tag->is_mandatory() && !tag->is_defined() )
				throw exception_t< Trait >(
					Trait::from_ascii( "Undefined child mandatory tag: \"" ) +
					tag->name() +
					Trait::from_ascii( "\". Where parent is: \"" ) +
					this->name() + Trait::from_ascii( "\". In file \"" ) +
					info.file_name() +
					Trait::from_ascii( "\" on line " ) +
					Trait::to_string( info.line_number() ) +
					Trait::from_ascii( "." ) );
		}
	}

	//! Called when string found.
	void on_string( const parser_info_t< Trait > & info,
		const typename Trait::string_t & str ) override
	{
		if( !this->is_defined_member_value() )
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

			m_value = format_t< bool, Trait >::from_string( info, str );

			this->set_defined();
		}
		else
			throw exception_t< Trait >(
				Trait::from_ascii( "Value for the tag \"" ) +
				this->name() +
				Trait::from_ascii( "\" already defined. In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) +
				Trait::from_ascii( "." ) );
	}

private:
	//! Value of the tag.
	bool m_value;
}; // class tag_scalar_t


//
// tag_scalar_t< Trait::string_t >
//

//! Tag with string value.
template< typename Trait >
class tag_scalar_t< typename Trait::string_t, Trait >
	:	public tag_t< Trait >
{
	static const typename Trait::pos_t c_max_string_length = 80;

public:
	//! Construct tag.
	explicit tag_scalar_t( const typename Trait::string_t & name,
		bool is_mandatory = false )
		:	tag_t< Trait >( name, is_mandatory )
		,	m_value( typename Trait::string_t() )
		,	m_constraint( nullptr )
	{
	}

	//! Construct tag.
	tag_scalar_t( tag_t< Trait > & owner, const typename Trait::string_t & name,
		bool is_mandatory = false )
		:	tag_t< Trait >( owner, name, is_mandatory )
		,	m_value( typename Trait::string_t() )
		,	m_constraint( nullptr )
	{
	}

	~tag_scalar_t()
	{
	}

	//! \return Value of the tag.
	const typename Trait::string_t &
	value() const
	{
		return m_value;
	}

	//! Set value.
	void
	set_value( const typename Trait::string_t & v )
	{
		if( m_constraint )
		{
			if( !m_constraint->check( v ) )
				throw exception_t< Trait >(
					Trait::from_ascii( "Invalid value: \"" ) +
					format_t< typename Trait::string_t, Trait >::to_string( v ) +
					Trait::from_ascii( "\". Value must match to the "
						"constraint in tag \"" ) +
					this->name() + Trait::from_ascii( "\"." ) );
		}

		m_value = v;

		this->set_defined();
	}

	/*!
		Query optional value.

		If isDefined() is true then \a receiver
		will be initialized with value of the tag,
		otherwise nothing with \a receiver will happen.
	*/
	void
	query_opt_value( typename Trait::string_t & receiver )
	{
		if( this->is_defined() )
			receiver = m_value;
	}

	//! Set constraint for the tag's value.
	void
	set_constraint( constraint_t< typename Trait::string_t > * c )
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
			result.push_back( const_t< Trait >::c_space );

			typename Trait::string_t value =
				format_t< typename Trait::string_t, Trait >::to_string( m_value );

			const typename Trait::pos_t sections =
				( value.length() / c_max_string_length +
				( value.length() % c_max_string_length > 0 ? 1 : 0 ) );

			if( sections )
			{
				const typename Trait::string_t spaces =
					typename Trait::string_t( this->name().length() + 2,
						const_t< Trait >::c_space );

				for( typename Trait::pos_t i = 0; i < sections; ++i )
				{
					if( i > 0 )
					{
						result.push_back( const_t< Trait >::c_carriage_return );

						result.append( typename Trait::string_t( indent,
							const_t< Trait >::c_tab ) );

						result.append( spaces );
					}

					const typename Trait::string_t tmp =
						to_cfgfile_format< Trait > (
							value.substr( i * c_max_string_length,
								c_max_string_length ) );

					result.append( tmp );
				}
			}
			else
			{
				result.push_back( const_t< Trait >::c_quotes );
				result.push_back( const_t< Trait >::c_quotes );
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

			typename Trait::string_t value =
				format_t< typename Trait::string_t, Trait >::to_string( m_value );
			value = to_cfgfile_format< Trait >( value );

			QDomText data = doc.createTextNode( value );

			this_element.appendChild( data );

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
		if( !this->is_defined_member_value() )
			throw exception_t< Trait >(
				Trait::from_ascii( "Undefined value of tag: \"" ) +
				this->name() + Trait::from_ascii( "\". In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) +
				Trait::from_ascii( "." ) );

		if( m_constraint )
		{
			if( !m_constraint->check( m_value ) )
				throw exception_t< Trait >(
					Trait::from_ascii( "Invalid value: \"" ) +
					m_value +
					Trait::from_ascii( "\". Value must match to the "
						"constraint in tag \"" ) +
					this->name() + Trait::from_ascii( "\". In file \"" ) +
					info.file_name() + Trait::from_ascii( "\" on line " ) +
					Trait::to_string( info.line_number() ) +
					Trait::from_ascii( "." ) );
		}

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

		const typename Trait::string_t value =
			format_t< typename Trait::string_t, Trait >::from_string( info, str );

		m_value.append( value );

		this->set_defined();
	}

private:
	//! Value of the tag.
	typename Trait::string_t m_value;
	//! Constraint.
	constraint_t< typename Trait::string_t > * m_constraint;
}; // class tag_scalar_t


#ifdef CFGFILE_QT_SUPPORT

//
// tag_scalar_t< QString >
//

//! Tag with QString value.
template< typename Trait >
class tag_scalar_t< QString, Trait >
	:	public tag_t< Trait >
{
	static const typename Trait::pos_t c_max_string_length = 80;

public:
	//! Construct tag.
	explicit tag_scalar_t( const typename Trait::string_t & name,
		bool is_mandatory = false )
		:	tag_t< Trait >( name, is_mandatory )
		,	m_value( QString() )
		,	m_constraint( nullptr )
	{
	}

	//! Construct tag.
	tag_scalar_t( tag_t< Trait > & owner, const typename Trait::string_t & name,
		bool is_mandatory = false )
		:	tag_t< Trait >( owner, name, is_mandatory )
		,	m_value( QString() )
		,	m_constraint( nullptr )
	{
	}

	~tag_scalar_t()
	{
	}

	//! \return Value of the tag.
	const QString &
	value() const
	{
		return m_value;
	}

	//! Set value.
	void
	set_value( const QString & v )
	{
		if( m_constraint )
		{
			if( !m_constraint->check( v ) )
				throw exception_t< Trait >(
					Trait::from_ascii( "Invalid value: \"" ) +
					typename Trait::string_t(
						format_t< QString, Trait >::to_string( v ) ) +
					Trait::from_ascii( "\". Value must match to the constraint "
						"in tag \"" ) +
					this->name() + Trait::from_ascii( "\"." ) );
		}

		m_value = v;

		this->set_defined();
	}

	/*!
		Query optional value.

		If isDefined() is true then \a receiver
		will be initialized with value of the tag,
		otherwise nothing with \a receiver will happen.
	*/
	void
	query_opt_value( QString & receiver )
	{
		if( this->is_defined() )
			receiver = m_value;
	}

	//! Set constraint for the tag's value.
	void
	set_constraint( constraint_t< QString > * c )
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
			result.push_back( const_t< Trait >::c_space );

			QString value = format_t< QString, Trait >::to_string( m_value );

			const typename Trait::pos_t sections =
				( value.length() / c_max_string_length +
				( value.length() % c_max_string_length > 0 ? 1 : 0 ) );

			if( sections )
			{
				const typename Trait::string_t spaces = typename Trait::string_t(
					this->name().length() + 2, const_t< Trait >::c_space );

				for( typename Trait::pos_t i = 0; i < sections; ++i )
				{
					if( i > 0 )
					{
						result.push_back( const_t< Trait >::c_carriage_return );

						result.append( typename Trait::string_t( indent,
							const_t< Trait >::c_tab ) );

						result.append( spaces );
					}

					const typename Trait::string_t tmp =
						to_cfgfile_format< Trait >(
							value.mid( i * c_max_string_length,
								c_max_string_length ) );

					result.append( tmp );
				}
			}
			else
			{
				result.push_back( const_t< Trait >::c_quotes );
				result.push_back( const_t< Trait >::c_quotes );
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

#ifdef CFGFILE_XML_SUPPORT
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

			typename Trait::string_t value =
				format_t< typename Trait::string_t, Trait >::to_string( m_value );
			value = to_cfgfile_format< Trait >( value );

			QDomText data = doc.createTextNode( value );

			this_element.appendChild( data );

			if( !this->children().empty() )
			{
				for( const tag_t< Trait > * tag : this->children() )
					tag->print( doc, &this_element );
			}
		}
	}
#endif // CFGFILE_XML_SUPPORT

	//! Called when tag parsing finished.
	void on_finish( const parser_info_t< Trait > & info ) override
	{
		if( m_constraint )
		{
			if( !m_constraint->check( m_value ) )
				throw exception_t< Trait >(
					Trait::from_ascii( "Invalid value: \"" ) +
					typename Trait::string_t( m_value ) +
					Trait::from_ascii( "\". Value must match to the "
						"constraint in tag \"" ) +
					this->name() + Trait::from_ascii( "\". In file \"" ) +
					info.file_name() + Trait::from_ascii( "\" on line " ) +
					Trait::to_string( info.line_number() ) +
					Trait::from_ascii( "." ) );
		}

		if( !this->is_defined_member_value() )
			throw exception_t< Trait >(
				Trait::from_ascii( "Undefined value of tag: \"" ) +
				this->name() + Trait::from_ascii( "\". In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) +
				Trait::from_ascii( "." ) );

		for( const tag_t< Trait > * tag : this->children() )
		{
			if( tag->is_mandatory() && !tag->is_defined() )
				throw exception_t< Trait >(
					Trait::from_ascii( "Undefined child mandatory tag: \"" ) +
					tag->name() + Trait::from_ascii( "\". Where parent is: \"" ) +
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
				Trait::from_ascii( "\" must be defined before any "
					"child tag. In file \"" ) +
				info.file_name() + Trait::from_ascii( "\" on line " ) +
				Trait::to_string( info.line_number() ) +
				Trait::from_ascii( "." ) );

		const QString value = format_t< QString, Trait >::from_string( info, str );

		m_value.append( value );

		this->set_defined();
	}

private:
	//! Value of the tag.
	QString m_value;
	//! Constraint.
	constraint_t< QString > * m_constraint;
}; // class tag_scalar_t< QString >

#endif // CFGFILE_QT_SUPPORT

} /* namespace cfgfile */

#endif // CFGFILE__TAG_SCALAR_HPP__INCLUDED
