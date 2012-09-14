
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

#ifndef QTCONFFILE__TAG_SCALAR_HPP__INCLUDED
#define QTCONFFILE__TAG_SCALAR_HPP__INCLUDED

//QtConfFile include.
#include <QtConfFile/private/Tag>
#include <QtConfFile/private/Constraint>
#include <QtConfFile/private/Format>
#include <QtConfFile/private/Utils>


namespace QtConfFile {

//
// TagScalar
//

//! Tag with scalar value.
template< class T >
class TagScalar
	:	public Tag
{
public:
	explicit TagScalar( const QString & name, bool isMandatory = false );
	TagScalar( Tag & owner, const QString & name, bool isMandatory = false );

	virtual ~TagScalar();

	//! \return Value of the tag.
	const T &
	value() const;

	//! Set value.
	void
	setValue( const T & v );

	/*!
		Query optional value.

		If isDefined() is true then \a receiver
		will be initialized with value of the tag,
		otherwise nothing with \a receiver will happen.
	*/
	void
	queryOptValue( T & receiver );

	//! Set constraint for the tag's value.
	void
	setConstraint( Constraint< T > * c );

	//! Print tag to the output.
	virtual QString print( int indent = 0 ) const;

protected:
	//! Called when tag parsing started.
	virtual void onStart( const ParserInfo & info );

	//! Called when tag parsing finished.
	virtual void onFinish( const ParserInfo & info );

	//! Called when string found.
	virtual void onString( const ParserInfo & info,
		const QString & str );

private:
	//! Value of the tag.
	T m_value;
	//! Constraint.
	Constraint< T > * m_constraint;
}; // class TagScalar


template< class T >
TagScalar< T >::TagScalar( const QString & name, bool isMandatory )
	:	Tag( name, isMandatory )
	,	m_constraint( 0 )
{
}

template< class T >
TagScalar< T >::TagScalar( Tag & owner, const QString & name, bool isMandatory )
	:	Tag( owner, name, isMandatory )
	,	m_constraint( 0 )
{
}

template< class T >
TagScalar< T >::~TagScalar()
{
}

template< class T >
const T &
TagScalar< T >::value() const
{
	return m_value;
}

template< class T >
void
TagScalar< T >::setValue( const T & v )
{
	if( m_constraint )
	{
		if( !m_constraint->check( v ) )
			throw Exception( QString( "Invalid value: \"%1\". "
				"Value must match to the constraint." )
					.arg( Format::toString( v ) ) );
	}

	m_value = v;

	setDefined();
}

template< class T >
void
TagScalar< T >::queryOptValue( T & receiver )
{
	if( isDefined() )
		receiver = m_value;
}

template< class T >
void
TagScalar< T >::setConstraint( Constraint< T > * c )
{
	m_constraint = c;
}

template< class T >
QString
TagScalar< T >::print( int indent ) const
{
	QString result;

	if( isDefined() )
	{
		for( int i = 0; i < indent; ++i )
			result.append( QLatin1String( "\t" ) );

		result.append( QLatin1String( "{" ) );
		result.append( name() );
		result.append( QLatin1String( " " ) );

		QString value = Format::toString( m_value );
		value = toQtConfFileFormat( value );

		result.append( value );

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
TagScalar< T >::onStart( const ParserInfo & info )
{
	Q_UNUSED( info )
}

template< class T >
void
TagScalar< T >::onFinish( const ParserInfo & info )
{
	Q_UNUSED( info )
}

template< class T >
void
TagScalar< T >::onString( const ParserInfo & info,
	const QString & str )
{
	T value = Format::fromString( info, str );

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

	m_value = value;

	setDefined();
}

} /* namespace QtConfFile */

#endif // QTCONFFILE__TAG_SCALAR_HPP__INCLUDED
