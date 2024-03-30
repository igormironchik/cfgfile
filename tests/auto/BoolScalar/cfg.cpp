
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Example include.
#include "cfg.hpp"


//
// Configuration
//


Configuration::Configuration()
	:	m_withTrue( false )
	,	m_withFalse( true )
{
}


//
// TagConfiguration
//

TagConfiguration::TagConfiguration()
	:	cfgfile::tag_no_value_t<>( "cfg", true )
	,	m_withTrue( *this, "withTrue", true )
	,	m_withFalse( *this, "withFalse", true )
{
}

TagConfiguration::TagConfiguration( const Configuration & cfg )
	:	cfgfile::tag_no_value_t<>( "cfg", true )
	,	m_withTrue( *this, "withTrue", true )
	,	m_withFalse( *this, "withFalse", true )
{
	m_withTrue.set_value( cfg.m_withTrue );
	m_withFalse.set_value( cfg.m_withFalse );

	set_defined();
}

Configuration
TagConfiguration::configuration() const
{
	Configuration cfg;

	cfg.m_withTrue = m_withTrue.value();
	cfg.m_withFalse = m_withFalse.value();

	return cfg;
}
