
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

#ifndef CFGFILE__TAG_VECTOR_OF_TAGS_HPP__INCLUDED
#define CFGFILE__TAG_VECTOR_OF_TAGS_HPP__INCLUDED

// cfgfile include.
#include "tag.hpp"
#include "exceptions.hpp"
#include "types.hpp"

// C++ include.
#include <vector>
#include <memory>


namespace cfgfile {

//
// tag_vector_of_tags_t
//

/*!
	Tag with multiple instancies of the given tag.

	Subordinate tag must have following constructor

	\code
		tag_t( const string_t & name, bool is_mandatory );
	\endcode
*/
template< class T >
class tag_vector_of_tags_t
	:	public tag_t
{
public:
	//! Type of the pointer to the subordinate tag,
	typedef std::shared_ptr< T > ptr_to_tag_t;
	//! Type of the vector of subordinate tags.
	typedef std::vector< ptr_to_tag_t > vector_of_tags_t;

	explicit tag_vector_of_tags_t( const string_t & name,
		bool is_mandatory = false )
		:	tag_t( name, is_mandatory )
	{
	}

	tag_vector_of_tags_t( tag_t & owner, const string_t & name,
		bool is_mandatory = false )
		:	tag_t( owner, name, is_mandatory )
	{
	}

	~tag_vector_of_tags_t()
	{
	}

	//! \return Amount of the subordinate tags.
	vector_of_tags_t::size_type
	size() const
	{
		return m_tags.size();
	}

	//! \return Subordinate tag at the given position.
	const T &
	at( vector_of_tags_t::size_type index ) const
	{
		return *m_tags.at( index );
	}

	//! \return Vector with subordinate tags.
	const vector_of_tags_t &
	values() const
	{
		return m_tags;
	}

	/*!
		Set value.

		Repeatly adds value to the end of vector.
	*/
	void
	set_value( ptr_to_tag_t & p )
	{
		m_tags.push_back( p );

		set_defined();
	}

	//! Set values.
	void
	set_values( const vector_of_tags_t & v )
	{
		m_tags = v;

		set_defined();
	}

	/*!
		Query optional values.

		If isDefined() is true then \a receiver
		will be initialized with values of the tag,
		otherwise nothing with \a receiver will happen.
	*/
	void
	query_opt_values( vector_of_tags_t & receiver )
	{
		if( is_defined() )
			receiver = m_tags;
	}

	//! \return List with children.
	const child_tags_list_t & children() const override
	{
		static const tag_t::child_tags_list_t empty;

		if( m_current )
			return m_current->children();
		else
			return empty;
	}

	//! Print tag to the output.
	string_t print( int indent = 0 ) const override
	{
		string_t result;

		if( is_defined() )
		{
			for( const ptr_to_tag_t & p : m_tags )
				result.push_back( p->print( indent ) );
		}

		return result;
	}

#if defined( CFGFILE_QSTRING_BUILD ) && defined( CFGFILE_XML_BUILD )
	//! Print tag to the output.
	void print( QDomDocument & doc, QDomElement * parent = 0 ) const override
	{
		if( is_defined() )
		{
			for( const ptr_to_tag_t & p : m_tags )
				p->print( doc, parent );
		}
	}
#endif

	//! Called when tag parsing started.
	void on_start( const parser_info_t & info ) override
	{
		m_current = std::make_shared< T > ( name(), is_mandatory() );
		m_current->set_parent( parent() );
		m_current->on_start( info );
	}

	//! Called when tag parsing finished.
	void on_finish( const parser_info_t & info ) override
	{
		m_current->on_finish( info );
		m_tags.push_back( m_current );
		m_current.reset();

		for( const tag_t * tag : children() )
		{
			if( tag->is_mandatory() && !tag->is_defined() )
				throw exception_t( string_t( SL( "Undefined child mandatory tag: \"" ) ) +
					tag->name() + SL( "\". Where parent is: \"" ) +
					name() + SL( "\". In file \"" ) +
					info.file_name() + SL( "\" on line " ) +
					info.line_number() + SL( "." ) );
		}

		set_defined();
	}

	//! Called when string found.
	void on_string( const parser_info_t & info,
		const string_t & str ) override
	{
		m_current->on_string( info, str );
	}

private:
	//! Vector of subordinate tags.
	vector_of_tags_t m_tags;
	//! Pointer to the current subordinate tag.
	ptr_to_tag_t m_current;
}; // class tag_vector_of_tags_t

} /* namespace cfgfile */

#endif // CFGFILE__TAG_VECTOR_OF_TAGS_HPP__INCLUDED
