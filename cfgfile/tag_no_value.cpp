
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

// cfgfile include.
#include <cfgfile/TagNoValue>
#include <cfgfile/Exceptions>

// Qt include.
#include <QDomDocument>
#include <QDomElement>


namespace cfgfile {

//
// TagNoValue
//

TagNoValue::TagNoValue( const QString & name, bool isMandatory )
	:	Tag( name, isMandatory )
{
}

TagNoValue::TagNoValue( Tag & owner, const QString & name, bool isMandatory )
	:	Tag( owner, name, isMandatory )
{
}


TagNoValue::~TagNoValue()
{
}

QString
TagNoValue::print( int indent ) const
{
	QString result;

	if( isDefined() )
	{
		for( int i = 0; i < indent; ++i )
			result.append( QLatin1String( "\t" ) );

		result.append( QLatin1String( "{" ) );
		result.append( name() );

		if( !children().isEmpty() )
		{
			result.append( QLatin1String( "\n" ) );

			foreach( Tag * tag, children() )
				result.append( tag->print( indent + 1 ) );

			for( int i = 0; i < indent; ++i )
				result.append( QLatin1String( "\t" ) );
		}

		result.append( QLatin1String( "}\n" ) );
	}

	return result;
}

void
TagNoValue::print( QDomDocument & doc, QDomElement * parent ) const
{
	if( isDefined() )
	{
		QDomElement thisElement = doc.createElement( name() );

		if( !parent )
			doc.appendChild( thisElement );
		else
			parent->appendChild( thisElement );

		if( !children().isEmpty() )
		{
			foreach( Tag * tag, children() )
				tag->print( doc, &thisElement );
		}
	}
}

void
TagNoValue::onStart( const ParserInfo & info )
{
	Tag::onStart( info );
}

void
TagNoValue::onFinish( const ParserInfo & info )
{
	foreach( Tag * tag, children() )
	{
		if( tag->isMandatory() && !tag->isDefined() )
			throw Exception( QString( "Undefined child mandatory tag: \"%1\". "
				"Where parent is: \"%2\". "
				"In file \"%3\" on line %4." )
					.arg( tag->name() )
					.arg( name() )
					.arg( info.fileName() )
					.arg( info.lineNumber() ) );
	}

	setDefined();
}

void
TagNoValue::onString( const ParserInfo & info, const QString & str )
{
	throw Exception( QString( "Tag \"%1\" doesn't allow any values. "
		"But we've got this: \"%2\". In file \"%3\" on line %4." )
			 .arg( name() )
			 .arg( str )
			 .arg( info.fileName() )
			 .arg( info.lineNumber() ) );
}

} /* namespace cfgfile */
