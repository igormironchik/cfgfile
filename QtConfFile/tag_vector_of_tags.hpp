
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

#ifndef QTCONFFILE__TAG_VECTOR_OF_TAGS_HPP__INCLUDED
#define QTCONFFILE__TAG_VECTOR_OF_TAGS_HPP__INCLUDED

// QtConfFile include.
#include <QtConfFile/private/Tag>
#include <QtConfFile/Exceptions>

// Qt include.
#include <QtCore/QVector>
#include <QtCore/QSharedPointer>


namespace QtConfFile {

//
// TagVectorOfTags
//

/*!
	Tag with multiple instancies of the given tag.

	Subordinate tag must have following constructor

	\code
		Tag( const QString & name, bool isMandatory );
	\endcode
*/
template< class T >
class TagVectorOfTags
	:	public Tag
{
public:
	//! Type of the pointer to the subordinate tag,
	typedef QSharedPointer< T > PointerToTag;
	//! Type of the vector of subordinate tags.
	typedef QVector< PointerToTag > VectorOfTags;

	explicit TagVectorOfTags( const QString & name, bool isMandatory = false );
	TagVectorOfTags( Tag & owner, const QString & name, bool isMandatory = false );

	virtual ~TagVectorOfTags();

	//! \return Amount of the subordinate tags.
	int
	size() const;

	//! \return Subordinate tag at the given position.
	const T &
	at( int index ) const;

	//! \return Vector with subordinate tags.
	const VectorOfTags &
	values() const;

	/*!
		Set value.

		Repeatly adds value to the end of vector.
	*/
	void
	setValue( PointerToTag & p );

	//! Set values.
	void
	setValues( const VectorOfTags & v );

	/*!
		Query optional values.

		If isDefined() is true then \a receiver
		will be initialized with values of the tag,
		otherwise nothing with \a receiver will happen.
	*/
	void
	queryOptValues( VectorOfTags & receiver );

	//! \return List with children.
	virtual const ChildTagsList & children() const;

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
	//! Vector of subordinate tags.
	VectorOfTags m_tags;
	//! Pointer to the current subordinate tag.
	PointerToTag m_current;
}; // class TagVectorOfTags


template< class T >
TagVectorOfTags< T >::TagVectorOfTags( const QString & name, bool isMandatory )
	:	Tag( name, isMandatory )
{
}

template< class T >
TagVectorOfTags< T >::TagVectorOfTags( Tag & owner, const QString & name,
	bool isMandatory )
	:	Tag( owner, name, isMandatory )
{
}

template< class T >
TagVectorOfTags< T >::~TagVectorOfTags()
{
}

template< class T >
int
TagVectorOfTags< T >::size() const
{
	return m_tags.size();
}

template< class T >
const T &
TagVectorOfTags< T >::at( int index ) const
{
	return *m_tags.at( index );
}

template< class T >
const typename TagVectorOfTags< T >::VectorOfTags &
TagVectorOfTags< T >::values() const
{
	return m_tags;
}

template< class T >
void
TagVectorOfTags< T >::setValue( PointerToTag & p )
{
	m_tags.push_back( p );

	setDefined();
}

template< class T >
void
TagVectorOfTags< T >::setValues( const VectorOfTags & v )
{
	m_tags = v;

	setDefined();
}

template< class T >
void
TagVectorOfTags< T >::queryOptValues( VectorOfTags & receiver )
{
	if( isDefined() )
		receiver = m_tags;
}

template< class T >
const Tag::ChildTagsList &
TagVectorOfTags< T >::children() const
{
	static const Tag::ChildTagsList empty;

	if( m_current )
		return m_current->children();
	else
		return empty;
}

template< class T >
QString
TagVectorOfTags< T >::print( int indent ) const
{
	QString result;

	if( isDefined() )
	{
		foreach( PointerToTag p, m_tags )
			result.append( p->print( indent ) );
	}

	return result;
}

template< class T >
void
TagVectorOfTags< T >::onStart( const ParserInfo & info )
{
	m_current = PointerToTag( new T( name(), isMandatory() ) );
	m_current->onStart( info );
}

template< class T >
void
TagVectorOfTags< T >::onFinish( const ParserInfo & info )
{
	m_current->onFinish( info );
	m_tags.push_back( m_current );
	m_current.clear();

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

	setDefined();
}

template< class T >
void
TagVectorOfTags< T >::onString( const ParserInfo & info,
	const QString & str )
{
	m_current->onString( info, str );
}

} /* namespace QtConfFile */

#endif // QTCONFFILE__TAG_VECTOR_OF_TAGS_HPP__INCLUDED
