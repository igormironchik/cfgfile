
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

#ifndef CFGFILE__UTILS_HPP__INCLUDED
#define CFGFILE__UTILS_HPP__INCLUDED

// QtCOnfFile include.
#include <cfgfile/private/Tag>

// Qt include.
#include <QtCore/QString>
#include <QtCore/QTextCodec>


namespace cfgfile {

//
// FileFormat
//

//! Format of the conf file.
enum FileFormat {
	//! cfgfile format.
	cfgfileFormat,
	//! XML format.
	XMLFormat
}; // enum FileFormat


//
// readcfgfile
//

//! Read cfgfile configuration file.
void
readcfgfile(
	//! Configuration tag.
	Tag & tag,
	//! File name.
	const QString & fileName,
	//! Text codec.
	QTextCodec * codec = QTextCodec::codecForLocale() );


//
// writecfgfile
//

//! Write cfgfile configuration file.
void
writecfgfile(
	//! Configuration tag.
	const Tag & tag,
	//! File name.
	const QString & fileName,
	//! Text codec.
	QTextCodec * codec = QTextCodec::codecForLocale(),
	//! Format of the file.
	FileFormat fmt = cfgfileFormat );

} /* namespace cfgfile */

#endif // CFGFILE__UTILS_HPP__INCLUDED
