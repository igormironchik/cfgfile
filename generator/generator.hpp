
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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
