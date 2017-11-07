
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

#ifndef TEST__CUSTOM_HPP__INCLUDED
#define TEST__CUSTOM_HPP__INCLUDED

// cfgfile include.
#include <cfgfile/all.hpp>


namespace cfg {

//
// custom_t
//

struct custom_t {
	custom_t()
		:	m_value( 0 )
	{
	}

	int m_value;
}; // struct custom_t


//
// tag_custom_t
//

template< typename Trait >
class tag_custom_t
	:	public cfgfile::tag_no_value_t< Trait >
{
public:
	tag_custom_t( cfgfile::tag_t< Trait > & owner,
		const typename Trait::string_t & name,
		bool is_mandatory = true )
		:	cfgfile::tag_no_value_t< Trait > ( owner, name, is_mandatory )
		,	m_value( *this, "value", true )
	{
	}

	tag_custom_t( const custom_t & cfg, cfgfile::tag_t< Trait > & owner,
		const typename Trait::string_t & name,
		bool is_mandatory = true )
		:	cfgfile::tag_no_value_t< Trait > ( owner, name, is_mandatory )
		,	m_value( *this, "value", true )
	{
		set_cfg( cfg );
	}

	custom_t get_cfg() const
	{
		custom_t c;

		c.m_value = m_value.value();

		return c;
	}

	void set_cfg( const custom_t & cfg )
	{
		m_value.set_value( cfg.m_value );

		this->set_defined();
	}

private:
	cfgfile::tag_scalar_t< int, Trait > m_value;
}; // class tag_custom_t

} /* namespace cfg */

#endif // TEST__CUSTOM_HPP__INCLUDED
