
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2012 - 2013 Igor Mironchik

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
#include <QtConfFile/private/StringFormat>

// Qt include.
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>


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

	//! Print tag to the output.
	virtual void print( QDomDocument & doc, QDomElement * parent = 0 ) const;

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
				"Value must match to the constraint in tag \"%2\"." )
					.arg( Format< T >::toString( v ) )
					.arg( name() ) );
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

		QString value = Format< T >::toString( m_value );
		value = toQtConfFileFormat( value );

		result.append( value );

		if( !children().isEmpty() )
		{
			result.append( QLatin1String( "\n" ) );

			foreach( Tag * tag, children() )
				result.append( tag->print( indent + 1 ) );

			for( int i = 0; i < indent; ++i )
				result.append( QLatin1String( "\t" ) );
		}

		result.append( QLatin1String( "}\n" ) );
	}

	return result;
}

template< class T >
void
TagScalar< T >::print( QDomDocument & doc, QDomElement * parent ) const
{
	if( isDefined() )
	{
		QDomElement thisElement = doc.createElement( name() );

		if( !parent )
			doc.appendChild( thisElement );
		else
			parent->appendChild( thisElement );

		QString value = Format< T >::toString( m_value );
		value = toQtConfFileFormat( value );

		QDomText data = doc.createTextNode( value );

		thisElement.appendChild( data );

		if( !children().isEmpty() )
		{
			foreach( Tag * tag, children() )
				tag->print( doc, &thisElement );
		}
	}
}

template< class T >
void
TagScalar< T >::onStart( const ParserInfo & info )
{
	Tag::onStart( info );
}

template< class T >
void
TagScalar< T >::onFinish( const ParserInfo & info )
{
	if( !isDefined() )
		throw Exception( QString( "Undefined value of tag: \"%1\". "
			"In file \"%2\" on line %3." )
				.arg( name() )
				.arg( info.fileName() )
				.arg( info.lineNumber() ) );

	foreach( Tag * tag, children() )
	{
		if( tag->isMandatory() && !tag->isDefined() )
			throw Exception( QString( "Undefined child mandatory tag: \"%1\". "
				"Where parent is: \"%2\". "
				"In file \"%3\" on line %4." )
					.arg( tag->name() )
					.arg( name() )
					.arg( info.fileName() )
					.arg( info.lineNumber() ) );
	}
}

template< class T >
void
TagScalar< T >::onString( const ParserInfo & info,
	const QString & str )
{
	if( !isDefined() )
	{
		if( isAnyChildDefined() )
			throw Exception( QString( "Value \"%1\" for tag \"%2\" "
				"must be defined before any child tag."
				"In file \"%3\" on line %4." )
					.arg( str )
					.arg( name() )
					.arg( info.fileName() )
					.arg( info.lineNumber() ) );

		T value = Format< T >::fromString( info, str );

		if( m_constraint )
		{
			if( !m_constraint->check( value ) )
				throw Exception( QString( "Invalid value: \"%1\". "
					"Value must match to the constraint in tag \"%2\". "
					"In file \"%3\" on line %4." )
						.arg( str )
						.arg( name() )
						.arg( info.fileName() )
						.arg( info.lineNumber() ) );
		}

		m_value = value;

		setDefined();
	}
	else
		throw Exception( QString( "Value for the tag \"%1\" already defined. "
			"In file \"%2\" on line %3." )
				.arg( name() )
				.arg( info.fileName() )
				.arg( info.lineNumber() ) );
}


//
// TagScalar< bool >
//

//! Tag with bool value.
template<>
class TagScalar< bool >
	:	public Tag
{
public:
	explicit TagScalar( const QString & name, bool isMandatory = false );
	TagScalar( Tag & owner, const QString & name, bool isMandatory = false );

	virtual ~TagScalar();

	//! \return Value of the tag.
	const bool &
	value() const;

	//! Set value.
	void
	setValue( const bool & v );

	/*!
		Query optional value.

		If isDefined() is true then \a receiver
		will be initialized with value of the tag,
		otherwise nothing with \a receiver will happen.
	*/
	void
	queryOptValue( bool & receiver );

	//! Print tag to the output.
	virtual QString print( int indent = 0 ) const;

	//! Print tag to the output.
	virtual void print( QDomDocument & doc, QDomElement * parent = 0 ) const;

	//! Called when tag parsing started.
	virtual void onStart( const ParserInfo & info );

	//! Called when tag parsing finished.
	virtual void onFinish( const ParserInfo & info );

	//! Called when string found.
	virtual void onString( const ParserInfo & info,
		const QString & str );

private:
	//! Value of the tag.
	bool m_value;
}; // class TagScalar


inline
TagScalar< bool >::TagScalar( const QString & name, bool isMandatory )
	:	Tag( name, isMandatory )
	,	m_value( false )
{
}

inline
TagScalar< bool >::TagScalar( Tag & owner, const QString & name, bool isMandatory )
	:	Tag( owner, name, isMandatory )
	,	m_value( false )
{
}

inline
TagScalar< bool >::~TagScalar()
{
}

inline
const bool &
TagScalar< bool >::value() const
{
	return m_value;
}

inline
void
TagScalar< bool >::setValue( const bool & v )
{
	m_value = v;

	setDefined();
}

inline
void
TagScalar< bool >::queryOptValue( bool & receiver )
{
	if( isDefined() )
		receiver = m_value;
}

inline
QString
TagScalar< bool >::print( int indent ) const
{
	QString result;

	if( isDefined() )
	{
		for( int i = 0; i < indent; ++i )
			result.append( QLatin1String( "\t" ) );

		result.append( QLatin1String( "{" ) );
		result.append( name() );
		result.append( QLatin1String( " " ) );

		QString value = Format< bool >::toString( m_value );
		value = toQtConfFileFormat( value );

		result.append( value );

		if( !children().isEmpty() )
		{
			result.append( QLatin1String( "\n" ) );

			foreach( Tag * tag, children() )
				result.append( tag->print( indent + 1 ) );

			for( int i = 0; i < indent; ++i )
				result.append( QLatin1String( "\t" ) );
		}

		result.append( QLatin1String( "}\n" ) );
	}

	return result;
}

inline
void
TagScalar< bool >::print( QDomDocument & doc, QDomElement * parent ) const
{
	if( isDefined() )
	{
		QDomElement thisElement = doc.createElement( name() );

		if( !parent )
			doc.appendChild( thisElement );
		else
			parent->appendChild( thisElement );

		QString value = Format< bool >::toString( m_value );
		value = toQtConfFileFormat( value );

		QDomText data = doc.createTextNode( value );

		thisElement.appendChild( data );

		if( !children().isEmpty() )
		{
			foreach( Tag * tag, children() )
				tag->print( doc, &thisElement );
		}
	}
}

inline
void
TagScalar< bool >::onStart( const ParserInfo & info )
{
	Q_UNUSED( info )
}

inline
void
TagScalar< bool >::onFinish( const ParserInfo & info )
{
	if( !isDefined() )
		throw Exception( QString( "Undefined value of tag: \"%1\". "
			"In file \"%2\" on line %3." )
				.arg( name() )
				.arg( info.fileName() )
				.arg( info.lineNumber() ) );

	foreach( Tag * tag, children() )
	{
		if( tag->isMandatory() && !tag->isDefined() )
			throw Exception( QString( "Undefined child mandatory tag: \"%1\". "
				"Where parent is: \"%2\". "
				"In file \"%3\" on line %4." )
					.arg( tag->name() )
					.arg( name() )
					.arg( info.fileName() )
					.arg( info.lineNumber() ) );
	}
}

inline
void
TagScalar< bool >::onString( const ParserInfo & info,
	const QString & str )
{
	if( !isDefined() )
	{
		if( isAnyChildDefined() )
			throw Exception( QString( "Value \"%1\" for tag \"%2\" "
				"must be defined before any child tag."
				"In file \"%3\" on line %4." )
					.arg( str )
					.arg( name() )
					.arg( info.fileName() )
					.arg( info.lineNumber() ) );

		m_value = Format< bool >::fromString( info, str );

		setDefined();
	}
	else
		throw Exception( QString( "Value for the tag \"%1\" already defined. "
			"In file \"%2\" on line %3." )
				.arg( name() )
				.arg( info.fileName() )
				.arg( info.lineNumber() ) );
}


//
// TagScalar< QString >
//

//! Tag with bool value.
template<>
class TagScalar< QString >
	:	public Tag
{
public:
	explicit TagScalar( const QString & name, bool isMandatory = false );
	TagScalar( Tag & owner, const QString & name, bool isMandatory = false );

	virtual ~TagScalar();

	//! \return Value of the tag.
	const QString &
	value() const;

	//! Set value.
	void
	setValue( const QString & v );

	/*!
		Query optional value.

		If isDefined() is true then \a receiver
		will be initialized with value of the tag,
		otherwise nothing with \a receiver will happen.
	*/
	void
	queryOptValue( QString & receiver );

	//! Set constraint for the tag's value.
	void
	setConstraint( Constraint< QString > * c );

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
	QString m_value;
	//! Constraint.
	Constraint< QString > * m_constraint;
}; // class TagScalar

inline
TagScalar< QString >::TagScalar( const QString & name, bool isMandatory )
	:	Tag( name, isMandatory )
	,	m_constraint( 0 )
{
}

inline
TagScalar< QString >::TagScalar( Tag & owner, const QString & name, bool isMandatory )
	:	Tag( owner, name, isMandatory )
	,	m_constraint( 0 )
{
}

inline
TagScalar< QString >::~TagScalar()
{
}

inline
const QString &
TagScalar< QString >::value() const
{
	return m_value;
}

inline
void
TagScalar< QString >::setValue( const QString & v )
{
	if( m_constraint )
	{
		if( !m_constraint->check( v ) )
			throw Exception( QString( "Invalid value: \"%1\". "
				"Value must match to the constraint in tag \"%2\"." )
					.arg( Format< QString >::toString( v ) )
					.arg( name() ) );
	}

	m_value = v;

	setDefined();
}

inline
void
TagScalar< QString >::queryOptValue( QString & receiver )
{
	if( isDefined() )
		receiver = m_value;
}

inline
void
TagScalar< QString >::setConstraint( Constraint< QString > * c )
{
	m_constraint = c;
}

static const int c_maxStringLength = 80;

inline
QString
TagScalar< QString >::print( int indent ) const
{
	QString result;

	if( isDefined() )
	{
		for( int i = 0; i < indent; ++i )
			result.append( QLatin1String( "\t" ) );

		result.append( QLatin1String( "{" ) );
		result.append( name() );
		result.append( QLatin1String( " " ) );

		QString value = Format< QString >::toString( m_value );

		const int sections = ( value.length() / c_maxStringLength +
			( value.length() % c_maxStringLength > 0 ? 1 : 0 ) );

		if( sections )
		{
			const QString spaces = QString( name().length() + 2,
				QLatin1Char( ' ' ) );

			for( int i = 0; i < sections; ++i )
			{
				if( i > 0 )
				{
					result.append( QLatin1String( "\n" ) );

					for( int i = 0; i < indent; ++i )
						result.append( QLatin1String( "\t" ) );

					result.append( spaces );
				}

				const QString tmp = toQtConfFileFormat(
					value.mid( i * c_maxStringLength, c_maxStringLength ) );

				result.append( tmp );
			}
		}
		else
			result.append( QLatin1String( "\"\"" ) );

		if( !children().isEmpty() )
		{
			result.append( QLatin1String( "\n" ) );

			foreach( Tag * tag, children() )
				result.append( tag->print( indent + 1 ) );

			for( int i = 0; i < indent; ++i )
				result.append( QLatin1String( "\t" ) );
		}

		result.append( QLatin1String( "}\n" ) );
	}

	return result;
}

inline
void
TagScalar< QString >::onStart( const ParserInfo & info )
{
	Tag::onStart( info );
}

inline
void
TagScalar< QString >::onFinish( const ParserInfo & info )
{
	if( m_constraint )
	{
		if( !m_constraint->check( m_value ) )
			throw Exception( QString( "Invalid value: \"%1\". "
				"Value must match to the constraint in tag \"%2\". "
				"In file \"%3\" on line %4." )
					.arg( m_value )
					.arg( name() )
					.arg( info.fileName() )
					.arg( info.lineNumber() ) );
	}

	if( !isDefined() )
		throw Exception( QString( "Undefined value of tag: \"%1\". "
			"In file \"%2\" on line %3." )
				.arg( name() )
				.arg( info.fileName() )
				.arg( info.lineNumber() ) );

	foreach( Tag * tag, children() )
	{
		if( tag->isMandatory() && !tag->isDefined() )
			throw Exception( QString( "Undefined child mandatory tag: \"%1\". "
				"Where parent is: \"%2\". "
				"In file \"%3\" on line %4." )
					.arg( tag->name() )
					.arg( name() )
					.arg( info.fileName() )
					.arg( info.lineNumber() ) );
	}
}

inline
void
TagScalar< QString >::onString( const ParserInfo & info,
	const QString & str )
{
	if( isAnyChildDefined() )
		throw Exception( QString( "Value \"%1\" for tag \"%2\" "
			"must be defined before any child tag."
			"In file \"%3\" on line %4." )
				.arg( str )
				.arg( name() )
				.arg( info.fileName() )
				.arg( info.lineNumber() ) );

	const QString value = Format< QString >::fromString( info, str );

	m_value.append( value );

	setDefined();
}

} /* namespace QtConfFile */

#endif // QTCONFFILE__TAG_SCALAR_HPP__INCLUDED
