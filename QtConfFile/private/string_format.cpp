
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2012 Igor Mironchik

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

// QtConfFile include.
#include <QtConfFile/private/StringFormat>


namespace QtConfFile {

static const QChar c_beginTag = QChar( '{' );
static const QChar c_endTag = QChar( '}' );
static const QChar c_quotes = QChar( '"' );
static const QChar c_carriageReturn = QChar( '\n' );
static const QChar c_lineFeed = QChar( '\r' );
static const QChar c_tab = QChar( '\t' );
static const QChar c_backSlash = QChar( '\\' );
static const QChar c_space = QChar( ' ' );
static const QString c_oneLineComment = QLatin1String( "||" );
static const QString c_startMultiLineComment = QLatin1String( "|#" );
static const QString c_finishMultiLineComment = QLatin1String( "#|" );

//
// toQtConfFileFormat
//

QString toQtConfFileFormat( const QString & what )
{
	if( what.isEmpty() )
		return QLatin1String( "\"\"" );

	if( !what.contains( c_beginTag ) &&
		!what.contains( c_endTag ) &&
		!what.contains( c_quotes ) &&
		!what.contains( c_carriageReturn ) &&
		!what.contains( c_lineFeed ) &&
		!what.contains( c_tab ) &&
		!what.contains( c_backSlash ) &&
		!what.contains( c_space ) &&
		!what.contains( c_oneLineComment ) &&
		!what.contains( c_startMultiLineComment ) &&
		!what.contains( c_finishMultiLineComment ) )
			return what;
	else
	{
		QString result;
		result.append( c_quotes );

		foreach( QChar ch, what )
		{
			if( ch == c_quotes )
				result.append( QLatin1String( "\\\"" ) );
			else if( ch == c_carriageReturn )
				result.append( QLatin1String( "\\n" ) );
			else if( ch == c_lineFeed )
				result.append( QLatin1String( "\\r" ) );
			else if( ch == c_tab )
				result.append( QLatin1String( "\\t" ) );
			else if( ch == c_backSlash )
				result.append( QLatin1String( "\\\\" ) );
			else
				result.append( ch );
		}

		result.append( c_quotes );

		return result;
	}
}

} /* namespace QtConfFile */
