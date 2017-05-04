
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

#ifndef CFGFILE__PARSER_INFO_HPP__INCLUDED
#define CFGFILE__PARSER_INFO_HPP__INCLUDED

// Qt include.
#include <QtCore/QString>


namespace cfgfile {

//
// ParserInfo
//

//! Information about parsed file and current state of parsing.
class ParserInfo {
public:
	ParserInfo( const QString & fileName, qint64 lineNumber,
		qint64 columnNumber );

	//! \return File name.
	const QString & fileName() const;

	//! \return Line number.
	qint64 lineNumber() const;

	//! \return Column number.
	qint64 columnNumber() const;

private:
	//! File name.
	QString m_fileName;
	//! Line number.
	qint64 m_lineNumber;
	//! Column number.
	qint64 m_columnNumber;
}; // class PraserInfo

} /* namespace cfgfile */

#endif // CFGFILE__PARSER_INFO_HPP__INCLUDED
