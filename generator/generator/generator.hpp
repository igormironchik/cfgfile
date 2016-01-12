
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2016 Igor Mironchik

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

#ifndef QTCONFFILE__GENERATOR__GENERATOR_HPP__INCLUDED
#define QTCONFFILE__GENERATOR__GENERATOR_HPP__INCLUDED

// Generator cfg include.
#include "../cfg/cfg.hpp"


QT_BEGIN_NAMESPACE
class QTextStream;
QT_END_NAMESPACE


namespace QtConfFile {

namespace Generator {

//
// Generator
//

//! Base generator.
class Generator {
public:
	Generator();
	virtual ~Generator();

	//! Generate.
	virtual void generate( QTextStream & stream ) const = 0;
}; // class Generator


//
// CppGenerator
//

//! C++ generator.
class CppGenerator
	:	public Generator
{
public:
	explicit CppGenerator( const Cfg::Model & model );
	~CppGenerator();

	//! Generate.
	virtual void generate( QTextStream & stream ) const;

private:
	//! Model.
	const Cfg::Model & m_model;
}; // class CppGenerator

} /* namespace Generator */

} /* namespace QtConfFile */

#endif // QTCONFFILE__GENERATOR__GENERATOR_HPP__INCLUDED
