
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

#ifndef CFGFILE__TAG_NO_VALUE_HPP__INCLUDED
#define CFGFILE__TAG_NO_VALUE_HPP__INCLUDED

// cfgfile include.
#include "tag.hpp"
#include "parser_info.hpp"
#include "const.hpp"
#include "exceptions.hpp"

#if defined( CFGFILE_QSTRING_BUILD ) && defined( CFGFILE_XML_BUILD )
// Qt include.
#include <QDomDocument>
#include <QDomElement>
#endif


namespace cfgfile {

//
// tag_no_value_t
//

//! Tag without a value.
class tag_no_value_t
	:	public tag_t
{
public:
	explicit tag_no_value_t( const string_t & name,
		bool is_mandatory = false )
		:	tag_t( name, is_mandatory )
	{
	}

	tag_no_value_t( tag_t & owner, const string_t & name,
		bool is_mandatory = false )
		:	tag_t( owner, name, is_mandatory )
	{
	}

	~tag_no_value_t()
	{
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
					tag->name() +
					SL( "\". Where parent is: \"" ) + name() +
					SL( "\". In file \"" ) + info.file_name() +
					SL( "\" on line " ) +
					pos_to_string( info.line_number() ) +
					SL( "." ) );
		}

		set_defined();
	}

	//! Called when string found.
	void on_string( const parser_info_t & info,
		const string_t & str ) override
	{
		throw exception_t( string_t( SL( "Tag \"" ) ) +
			name() + SL( "\" doesn't allow any values. "
				"But we've got this: \"" ) +
			str + SL( "\". In file \"" ) + info.file_name() +
			SL( "\" on line " ) +
			pos_to_string( info.line_number() ) +
			SL( "." ) );
	}
}; // class tag_no_value_t

} /* namespace cfgfile */

#endif // CFGFILE__TAG_NO_VALUE_HPP__INCLUDED
