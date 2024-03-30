
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef CFGFILE__CONSTRAINT_HPP__INCLUDED
#define CFGFILE__CONSTRAINT_HPP__INCLUDED


namespace cfgfile {

//
// constraint_t
//

//! Base class for constraints.
template< class T >
class constraint_t {
public:
	//! Construct constraint.
	constraint_t() {}

	virtual ~constraint_t() {}

	/*!
		Check value for correctness.

		\return Was check successful?
		\retval true If all is OK.
		\retval false If value doen't correspond to the constraint.
	*/
	virtual bool
	check( const T & value ) const = 0;
}; // class constraint_t

} /* namespace cfgfile */

#endif // CFGFILE__CONSTRAINT_HPP__INCLUDED
