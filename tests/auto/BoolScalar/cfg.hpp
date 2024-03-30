
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef CFGFILE__TEST__BOOL_SCALAR__CFG_HPP__INCLUDED
#define CFGFILE__TEST__BOOL_SCALAR__CFG_HPP__INCLUDED

// cfgfile include.
#include <cfgfile/all.hpp>


//
// Configuration
//

/*!
	Configuration that will be loaded from file and savend into file.

	\verbatim
	{cfg
		{withTrue "true"}
		{withFalse "false"}
	}
	\endverbatim
*/
struct Configuration {
	Configuration();

	bool m_withTrue;
	bool m_withFalse;
}; // class Configuration


//
// TagConfiguration
//

/*!
	Tag with configuration:

	\verbatim
	{cfg
		{withTrue "true"}
		{withFalse "false"}
	}
	\endverbatim
*/
class TagConfiguration
	:	public cfgfile::tag_no_value_t<>
{
public:
	//! For reading.
	TagConfiguration();
	//! For writing.
	explicit TagConfiguration( const Configuration & cfg );

	//! \return Configuration.
	Configuration
	configuration() const;

private:
	cfgfile::tag_scalar_t< bool > m_withTrue;
	cfgfile::tag_scalar_t< bool > m_withFalse;
}; // class TagConfiguration

#endif // CFGFILE__TEST__BOOL_SCALAR__CFG_HPP__INCLUDED
