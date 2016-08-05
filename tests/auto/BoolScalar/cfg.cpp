
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2013 Igor Mironchik

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

Configuration &
Configuration::operator = ( const Configuration & other )
{
	if( this != &other )
	{
		m_withTrue = other.m_withTrue;
		m_withFalse = other.m_withFalse;
	}

	return *this;
}


//
// TagConfiguration
//

TagConfiguration::TagConfiguration()
	:	QtConfFile::TagNoValue( QLatin1String( "cfg" ), true )
	,	m_withTrue( *this, QLatin1String( "withTrue" ), true )
	,	m_withFalse( *this, QLatin1String( "withFalse" ), true )
{
}

TagConfiguration::TagConfiguration( const Configuration & cfg )
	:	QtConfFile::TagNoValue( QLatin1String( "cfg" ), true )
	,	m_withTrue( *this, QLatin1String( "withTrue" ), true )
	,	m_withFalse( *this, QLatin1String( "withFalse" ), true )
{
	m_withTrue.setValue( cfg.m_withTrue );
	m_withFalse.setValue( cfg.m_withFalse );

	setDefined();
}

Configuration
TagConfiguration::configuration() const
{
	Configuration cfg;

	cfg.m_withTrue = m_withTrue.value();
	cfg.m_withFalse = m_withFalse.value();

	return cfg;
}
