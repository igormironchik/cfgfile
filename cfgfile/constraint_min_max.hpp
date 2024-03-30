
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef CFGFILE__CONSTRAINT_MIN_MAX_HPP__INCLUDED
#define CFGFILE__CONSTRAINT_MIN_MAX_HPP__INCLUDED

// cfgfile include.
#include "constraint.hpp"


namespace cfgfile {

//
// constraint_min_max_t
//

/*!
	Constraint that allowed values to be in the given range:
	min <= value <= max.
*/
template< class T >
class constraint_min_max_t final
	:	public constraint_t< T >
{
public:
	//! Construct constraint.
	constraint_min_max_t( const T & min, const T & max )
		:	m_min( min )
		,	m_max( max )
	{
	}

	~constraint_min_max_t()
	{
	}

	/*!
		Check value for correctness.

		\return Was check successful?
		\retval true If all is OK.
		\retval false If value doen't correspond to the constraint.
	*/
	bool check( const T & value ) const override
	{
		return ( m_min <= value && value <= m_max );
	}

private:
	//! Minimum value.
	T m_min;
	//! MAximum value.
	T m_max;
}; // class constraint_min_max_t

} /* namespace cfgfile */

#endif // CFGFILE__CONSTRAINT_MIN_MAX_HPP__INCLUDED
