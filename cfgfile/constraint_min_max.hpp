
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
