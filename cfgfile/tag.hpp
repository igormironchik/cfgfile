
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

#ifndef CFGFILE__TAG_HPP__INCLUDED
#define CFGFILE__TAG_HPP__INCLUDED

#if defined( CFGFILE_QSTRING_BUILD ) && defined( CFGFILE_XML_BUILD )
// Qt include.
#include <QDomDocument>
#include <QDomElement>
#endif

// cfgfile include.
#include "parser_info.hpp"
#include "types.hpp"

// C++ include.
#include <list>
#include <algorithm>


namespace cfgfile {

//
// tag_t
//

//! Base class for the tags in the configuration file.
class tag_t {
public:
    friend class parser_t;

    //! List with children.
    typedef std::list< tag_t* > child_tags_list_t;

    explicit tag_t( const string_t & name, bool is_mandatory = false )
		:   m_name( name )
		,   m_is_mandatory( is_mandatory )
		,   m_is_defined( false )
		,	m_parent( nullptr )
		,	m_line_number( -1 )
		,	m_column_number( -1 )
	{
	}

    tag_t( tag_t & owner, const string_t & name, bool is_mandatory = false )
		:   m_name( name )
		,   m_is_mandatory( is_mandatory )
		,   m_is_defined( false )
		,	m_parent( nullptr )
		,	m_line_number( -1 )
		,	m_column_number( -1 )
	{
		owner.add_child( *this );
	}

    virtual ~tag_t()
	{
	}

    //! Add child tag.
    void add_child( tag_t & tag )
	{
		if( std::find( m_child_tags.cbegin(), m_child_tags.cend(), &tag ) ==
			m_child_tags.cend() )
		{
			m_child_tags.push_back( &tag );

			tag.set_parent( this );
		}
	}

    //! Remove child tag.
    void remove_child( tag_t & tag )
	{
		auto it = std::find( m_child_tags.cbegin(), m_child_tags.cend(), &tag );

		if( it != m_child_tags.cend() )
		{
			m_child_tags.erase( it );

			tag.set_parent( nullptr );
		}
	}

	//! \return Parent tag.
	const tag_t * parent() const
	{
		return m_parent;
	}

    //! \return Name of the tag.
    const string_t & name() const
	{
		return m_name;
	}

    //! \return Is this tag mandatory?
    bool is_mandatory() const
	{
		return m_is_mandatory;
	}

    //! \return Is this tag defined?
    bool is_defined() const
	{
		for( const tag_t * tag : children() )
		{
			if( tag->is_mandatory() && !tag->is_defined() )
				return false;
		}

		return m_is_defined;
	}

    //! Set "defined" property.
    void set_defined( bool on = true )
	{
		m_is_defined = on;
	}

	//! \return Line number.
	pos_t line_number() const
	{
		return m_line_number;
	}

	//! \return Column number.
	pos_t column_number() const
	{
		return m_column_number;
	}

	//! \return List with children.
	virtual const child_tags_list_t & children() const
	{
		return m_child_tags;
	}

	//! Print tag to the output.
	virtual string_t print( int indent = 0 ) const = 0;

#if defined( CFGFILE_QSTRING_BUILD ) && defined( CFGFILE_XML_BUILD )
	//! Print tag to the output.
	virtual void print( QDomDocument & doc,
		QDomElement * parent = 0 ) const = 0;
#endif

    //! Called when tag parsing started.
	virtual void on_start( const parser_info_t< Trait > & info )
	{
		m_line_number = info.line_number();
		m_column_number = info.column_number();
	}

    //! Called when tag parsing finished.
	virtual void on_finish( const parser_info_t< Trait > & info ) = 0;

	//! Called when string found.
	virtual void on_string( const parser_info_t< Trait > & info,
		const string_t & str ) = 0;

protected:
	template< class T > friend class tag_vector_of_tags_t;

	//! Set parent tag.
	void set_parent( const tag_t * p )
	{
		m_parent = p;
	}

	//! \return Is any child tag defined?
	bool is_any_child_defined() const
	{
		if( std::any_of( m_child_tags.cbegin(), m_child_tags.cend(),
			[] ( tag_t * tag ) { return ( tag->is_defined() ); } ) )
				return true;

		return false;
	}

private:
    DISABLE_COPY( tag_t )

	//! Name.
	string_t m_name;
	//! Is tag mandatory?
    bool m_is_mandatory;
	//! Is tag defined?
    bool m_is_defined;
	//! Children.
    child_tags_list_t m_child_tags;
	//! Parent.
	const tag_t * m_parent;
	//! Line number.
	pos_t m_line_number;
	//! Column number.
	pos_t m_column_number;
}; // class tag_t

} /* namespace cfgfile */

#endif // CFGFILE__TAG_HPP__INCLUDED
