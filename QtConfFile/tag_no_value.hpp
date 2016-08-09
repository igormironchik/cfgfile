
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2012-2016 Igor Mironchik

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

#ifndef QTCONFFILE__TAG_NO_VALUE_HPP__INCLUDED
#define QTCONFFILE__TAG_NO_VALUE_HPP__INCLUDED

// QtConfFile include.
#include <QtConfFile/private/Tag>


namespace QtConfFile {

//
// TagNoValue
//

//! Tag without a value.
class TagNoValue
	:	public Tag
{
public:
	explicit TagNoValue( const QString & name, bool isMandatory = false );
	TagNoValue( Tag & owner, const QString & name, bool isMandatory = false );

	virtual ~TagNoValue();

	//! Print tag to the output.
	virtual QString print( int indent = 0 ) const;

	//! Print tag to the output.
	virtual void print( QDomDocument & doc, QDomElement * parent = 0 ) const;

	//! Called when tag parsing started.
	virtual void onStart( const ParserInfo & info );

	//! Called when tag parsing finished.
	virtual void onFinish( const ParserInfo & info );

	//! Called when string found.
	virtual void onString( const ParserInfo & info,
		const QString & str );
}; // class TagNoValue

} /* namespace QtConfFile */

#endif // QTCONFFILE__TAG_NO_VALUE_HPP__INCLUDED
