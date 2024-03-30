
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef CFGFILE__CONSTRAINT_ONE_OF_HPP__INCLUDED
#define CFGFILE__CONSTRAINT_ONE_OF_HPP__INCLUDED

// cfgfile include.
#include "constraint.hpp"

// C++ include.
#include <set>


namespace cfgfile {

//
// constraint_one_of_t
//

//! Constraint that allowed values to be in the given list of values.
template< class T >
class constraint_one_of_t final
	:	public constraint_t< T >
{
public:
	//! Construct constraint.
	constraint_one_of_t()
	{
	}

	~constraint_one_of_t()
	{
	}

	//! Add value to the list of values of the constraint.
	void
	add_value( const T & value )
	{
		m_list.insert( value );
	}

	//! Add value to the list of values of the constraint.
	void
	remove_value( const T & value )
	{
		m_list.erase( value );
	}

	/*!
		Check value for correctness.

		\return Was check successful?
		\retval true If all is OK.
		\retval false If value doen't correspond to the constraint.
	*/
	bool check( const T & value ) const override
	{
		return ( m_list.find( value ) != m_list.cend() );
	}

private:
	//! List of allowed values.
	std::set< T > m_list;
}; // class constraint_one_of_t

} /* namespace cfgfile */

#endif // CFGFILE__CONSTRAINT_ONE_OF_HPP__INCLUDED
