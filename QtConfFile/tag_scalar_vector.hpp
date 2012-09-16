
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

#ifndef QTCONFFILE__TAG_SCALAR_VECTOR_HPP__INCLUDED
#define QTCONFFILE__TAG_SCALAR_VECTOR_HPP__INCLUDED

//QtConfFile include.
#include <QtConfFile/private/Tag>
#include <QtConfFile/private/Constraint>
#include <QtConfFile/private/Format>
#include <QtConfFile/private/StringFormat>

// Qt include.
#include <QtCore/QVector>


namespace QtConfFile {

//
// TagScalarVector
//

//! Tag with multiple scalar values.
template< class T >
class TagScalarVector
	:	public Tag
{
public:
	typedef QVector< T > ValuesVector;

	explicit TagScalarVector( const QString & name, bool isMandatory = false );
	TagScalarVector( Tag & owner, const QString & name, bool isMandatory = false );

	virtual ~TagScalarVector();

	//! \return Amount of the values.
	int
	size() const;

	//! \return Value with the given index.
	const T &
	at( int index ) const;

	//! \return All values.
	const ValuesVector &
	values() const;

	/*!
		Set value.

		Repeatly adds value to the end of vector.
	*/
	void
	setValue( const T & v );

	//! Set values.
	void
	setValues( const ValuesVector & v );

	/*!
		Query optional values.

		If isDefined() is true then \a receiver
		will be initialized with values of the tag,
		otherwise nothing with \a receiver will happen.
	*/
	void
	queryOptValues( ValuesVector & receiver );

	//! Set constraint for the tag's value.
	void
	setConstraint( Constraint< T > * c );

	//! Print tag to the output.
	virtual QString print( int indent = 0 ) const;

	//! Called when tag parsing started.
	virtual void onStart( const ParserInfo & info );

	//! Called when tag parsing finished.
	virtual void onFinish( const ParserInfo & info );

	//! Called when string found.
	virtual void onString( const ParserInfo & info,
		const QString & str );

private:
	//! Value of the tag.
	ValuesVector m_values;
	//! Constraint.
	Constraint< T > * m_constraint;
}; // class TagScalarVector


template< class T >
TagScalarVector< T >::TagScalarVector( const QString & name, bool isMandatory )
	:	Tag( name, isMandatory )
	,	m_constraint( 0 )
{
}

template< class T >
TagScalarVector< T >::TagScalarVector( Tag & owner, const QString & name, bool isMandatory )
	:	Tag( owner, name, isMandatory )
	,	m_constraint( 0 )
{
}

template< class T >
TagScalarVector< T >::~TagScalarVector()
{
}

template< class T >
int
TagScalarVector< T >::size() const
{
	return m_values.size();
}

template< class T >
const T &
TagScalarVector< T >::at( int index ) const
{
	return m_values.at( index );
}

template< class T >
const typename TagScalarVector< T >::ValuesVector &
TagScalarVector< T >::values() const
{
	return m_values;
}

template< class T >
void
TagScalarVector< T >::setValue( const T & v )
{
	if( m_constraint )
	{
		if( !m_constraint->check( v ) )
			throw Exception( QString( "Invalid value: \"%1\". "
				"Value must match to the constraint." )
					.arg( Format< T >::toString( v ) ) );
	}

	m_values.push_back( v );

	setDefined();
}

template< class T >
void
TagScalarVector< T >::setValues( const ValuesVector & v )
{
	m_values = v;

	setDefined();
}

template< class T >
void
TagScalarVector< T >::queryOptValues( ValuesVector & receiver )
{
	if( isDefined() )
		receiver = m_values;
}

template< class T >
void
TagScalarVector< T >::setConstraint( Constraint< T > * c )
{
	m_constraint = c;
}

template< class T >
QString
TagScalarVector< T >::print( int indent ) const
{
	QString result;

	if( isDefined() )
	{
		for( int i = 0; i < indent; ++i )
			result.append( QLatin1String( "\t" ) );

		result.append( QLatin1String( "{" ) );
		result.append( name() );

		foreach( T v, m_values )
		{
			result.append( QLatin1String( " " ) );

			QString value = Format< T >::toString( v );
			value = toQtConfFileFormat( value );

			result.append( value );
		}

		if( !children().isEmpty() )
		{
			result.append( QLatin1String( "\r\n" ) );

			foreach( Tag * tag, children() )
				result.append( tag->print( indent + 1 ) );

			for( int i = 0; i < indent; ++i )
				result.append( QLatin1String( "\t" ) );
		}

		result.append( QLatin1String( "}\r\n" ) );
	}

	return result;
}

template< class T >
void
TagScalarVector< T >::onStart( const ParserInfo & info )
{
	Q_UNUSED( info )
}

template< class T >
void
TagScalarVector< T >::onFinish( const ParserInfo & info )
{
	foreach( Tag * tag, children() )
	{
		if( tag->isMandatory() && !tag->isDefined() )
			throw Exception( QString( "Undefined mandatory tag: \"%1\". "
				"In file \"%2\" on line %3." )
					.arg( tag->name() )
					.arg( info.fileName() )
					.arg( info.lineNumber() ) );
	}
}

template< class T >
void
TagScalarVector< T >::onString( const ParserInfo & info,
	const QString & str )
{
	T value = Format< T >::fromString( info, str );

	if( m_constraint )
	{
		if( !m_constraint->check( value ) )
			throw Exception( QString( "Invalid value: \"%1\". "
				"Value must match to the constraint. "
				"In file \"%2\" on line %3." )
					.arg( str )
					.arg( info.fileName() )
					.arg( info.lineNumber() ) );
	}

	m_values.push_back( value );

	setDefined();
}

} /* namespace QtConfFile */

#endif // QTCONFFILE__TAG_SCALAR_VECTOR_HPP__INCLUDED
