
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

#ifndef CFGFILE__GENERATOR__GENERATOR_HPP__INCLUDED
#define CFGFILE__GENERATOR__GENERATOR_HPP__INCLUDED

// generator_t cfg include.
#include "cfg.hpp"

// C++ include.
#include <ostream>


namespace cfgfile {

namespace generator {

//
// generator_t
//

//! Base generator.
class generator_t {
public:
	generator_t();
	virtual ~generator_t();

	//! Generate.
	virtual void generate( std::ostream & stream ) const = 0;
}; // class generator_t


//
// cpp_generator_t
//

//! C++ generator.
class cpp_generator_t
	:	public generator_t
{
public:
	explicit cpp_generator_t( const cfg::model_t & model );
	~cpp_generator_t();

	//! Generate.
	void generate( std::ostream & stream ) const override;

private:
	//! Model.
	const cfg::model_t & m_model;
}; // class cpp_generator_t

} /* namespace generator */

} /* namespace cfgfile */

#endif // CFGFILE__GENERATOR__GENERATOR_HPP__INCLUDED
