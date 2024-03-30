
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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
