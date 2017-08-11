
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

// Example include.
#include "cfg.hpp"

// Qt include.
#include <QtCore/QSharedPointer>


//
// Configuration::Pair
//

Configuration::Pair::Pair()
	:	m_intValue( 0 )
{
}

Configuration::Pair::Pair( const QString & stringValue, int intValue )
	:	m_stringValue( stringValue )
	,	m_intValue( intValue )
{
}


//
// Configuration
//


Configuration::Configuration()
	:	m_intValue( 0 )
{
}


//
// TagVecOfTags
//


TagVecOfTags::TagVecOfTags( const QString & name, bool isMandatory )
	:	cfgfile::tag_no_value_t< cfgfile::qstring_trait_t >( name, isMandatory )
	,	m_stringValue( *this, QLatin1String( "stringValue" ), true )
	,	m_intValue( *this, QLatin1String( "intValue" ), true )
{
}

TagVecOfTags::TagVecOfTags( const Configuration::Pair & pair )
	:	cfgfile::tag_no_value_t< cfgfile::qstring_trait_t >( QLatin1String( "vecOfTags" ), true )
	,	m_stringValue( *this, QLatin1String( "stringValue" ), true )
	,	m_intValue( *this, QLatin1String( "intValue" ), true )
{
	m_stringValue.set_value( pair.m_stringValue );
	m_intValue.set_value( pair.m_intValue );

	set_defined();
}


Configuration::Pair
TagVecOfTags::pair() const
{
	Configuration::Pair pair;

	pair.m_stringValue = m_stringValue.value();
	pair.m_intValue = m_intValue.value();

	return pair;
}


//
// TagConfiguration
//

TagConfiguration::TagConfiguration()
	:	cfgfile::tag_no_value_t< cfgfile::qstring_trait_t >( QLatin1String( "cfg" ), true )
	,	m_stringValue( *this, QLatin1String( "stringValue" ), true )
	,	m_listOfStringValues( *this, QLatin1String( "listOfStringValues" ), true )
	,	m_constriantForIntValue( 0, 100 )
	,	m_intValue( *this, QLatin1String( "intValue" ), true )
	,	m_vecOfTags( *this, QLatin1String( "vecOfTags" ), true )
{
	m_constraintForListOfStringValues.add_value( QLatin1String( "str1" ) );
	m_constraintForListOfStringValues.add_value( QLatin1String( "str2" ) );
	m_constraintForListOfStringValues.add_value( QLatin1String( "str3" ) );

	m_listOfStringValues.set_constraint( &m_constraintForListOfStringValues );
	m_intValue.set_constraint( &m_constriantForIntValue );
}

TagConfiguration::TagConfiguration( const Configuration & cfg )
	:	cfgfile::tag_no_value_t< cfgfile::qstring_trait_t >( QLatin1String( "cfg" ), true )
	,	m_stringValue( *this, QLatin1String( "stringValue" ), true )
	,	m_listOfStringValues( *this, QLatin1String( "listOfStringValues" ), true )
	,	m_constriantForIntValue( 0, 100 )
	,	m_intValue( *this, QLatin1String( "intValue" ), true )
	,	m_vecOfTags( *this, QLatin1String( "vecOfTags" ), true )
{
	m_constraintForListOfStringValues.add_value( QLatin1String( "str1" ) );
	m_constraintForListOfStringValues.add_value( QLatin1String( "str2" ) );
	m_constraintForListOfStringValues.add_value( QLatin1String( "str3" ) );

	m_listOfStringValues.set_constraint( &m_constraintForListOfStringValues );
	m_intValue.set_constraint( &m_constriantForIntValue );

	m_stringValue.set_value( cfg.m_stringValue );

	foreach( QString str, cfg.m_listOfStringValues )
		m_listOfStringValues.set_value( str );

	m_intValue.set_value( cfg.m_intValue );

	foreach( Configuration::Pair pair, cfg.m_vectorOfTags )
	{
		std::shared_ptr< TagVecOfTags > p( new TagVecOfTags( pair ) );
		m_vecOfTags.set_value( p );
	}

	set_defined();
}

Configuration
TagConfiguration::configuration() const
{
	Configuration cfg;

	cfg.m_stringValue = m_stringValue.value();
	cfg.m_listOfStringValues = m_listOfStringValues.values();
	cfg.m_intValue = m_intValue.value();

	foreach( auto p, m_vecOfTags.values() )
		cfg.m_vectorOfTags.push_back( p->pair() );

	return cfg;
}
