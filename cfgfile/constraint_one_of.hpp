
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
