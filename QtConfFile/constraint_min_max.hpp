
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2012 Igor Mironchik

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

#ifndef QTCONFFILE__CONSTRAINT_MIN_MAX_HPP__INCLUDED
#define QTCONFFILE__CONSTRAINT_MIN_MAX_HPP__INCLUDED

// QtConfFile include.
#include <QtConfFile/private/Constraint>


namespace QtConfFile {

//
// ConstraintMinMax
//

/*!
	Constraint that allowed values to be in the given range:
	min <= value <= max.
*/
template< class T >
class ConstraintMinMax
	:	public Constraint< T >
{
public:
	ConstraintMinMax( const T & min, const T & max );

	virtual ~ConstraintMinMax();

	/*!
		Check value for correctness.

		\return Was check successful?
		\retval true If all is OK.
		\retval false If value doen't correspond to the constraint.
	*/
	virtual bool
	check( const T & value ) const;

private:
	//! Minimum value.
	T m_min;
	//! MAximum value.
	T m_max;
}; // class ConstraintMinMax


template< class T >
ConstraintMinMax< T >::ConstraintMinMax( const T & min, const T & max )
	:	m_min( min )
	,	m_max( max )
{
}

template< class T >
ConstraintMinMax< T >::~ConstraintMinMax()
{
}

template< class T >
bool
ConstraintMinMax< T >::check( const T & value ) const
{
	return ( m_min <= value && value <= m_max );
}

} /* namespace QtConfFile */

#endif // QTCONFFILE__CONSTRAINT_MIN_MAX_HPP__INCLUDED
