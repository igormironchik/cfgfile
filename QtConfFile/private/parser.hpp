
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

#ifndef QTCONFFILE__PRIVATE__PARSER_HPP__INCLUDED
#define QTCONFFILE__PRIVATE__PARSER_HPP__INCLUDED

// Qt include.
#include <QtCore/QScopedPointer>

QT_BEGIN_NAMESPACE

class QDomDocument;

QT_END_NAMESPACE


namespace QtConfFile {

class InputStream;
class Tag;


//
// Parser
//

//! Parser of the configuration file.
class Parser {
public:
	Parser( Tag & tag, InputStream & stream );

	Parser( Tag & tag, const QDomDocument & dom );

	~Parser();

    /*!
        Parse input stream.
        \throw Exception on errors.
    */
	void parse( const QString & fileName );

private:
    Q_DISABLE_COPY( Parser )

    class ParserPrivate;
    QScopedPointer< ParserPrivate > d;
}; // class Parser

} /* namespace QtConfFile */

#endif // QTCONFFILE__PRIVATE__PARSER_HPP__INCLUDED
