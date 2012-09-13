
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

#ifndef QTCONFFILE__PRIVATE__TAG_HPP__INCLUDED
#define QTCONFFILE__PRIVATE__TAG_HPP__INCLUDED

// Qt include.
#include <QtCore/QList>
#include <QtCore/QScopedPointer>
#include <QtCore/QString>


namespace QtConfFile {

class Lexeme;


//
// Tag
//

//! Base class for the tags in the configuration file.
class Tag {
public:
    friend class Parser;

    //! List with children.
    typedef QList< Tag* > ChildTagsList;

    explicit Tag( const QString & name, bool isMandatory = false );
    Tag( Tag & owner, const QString & name, bool isMandatory = false );

    virtual ~Tag();

    //! Add child tag.
    void addChild( Tag & tag );

    //! Remove child tag.
    void removeChild( Tag & tag );

    //! \return List with children.
    const ChildTagsList & children() const;

    //! \return Name of the tag.
    const QString & name() const;

    //! \return Is this tag mandatory?
    bool isMandatory() const;

    //! \return Is this tag defined?
    bool isDefined() const;

    //! Set "defined" property.
    void setDefined( bool on = true );

	//! Print tag to the output.
	virtual QString print( int indent = 0 ) const = 0;

protected:
    //! Called when tag parsing started.
    virtual void onStart() = 0;

    //! Called when tag parsing finished.
    virtual void onFinish() = 0;

	//! Called when string found.
	virtual void onString( const QString & str ) = 0;

private:
    Q_DISABLE_COPY( Tag )

    class TagPrivate;
    QScopedPointer< TagPrivate > d;
}; // class Tag

} /* namespace QtConfFile */

#endif // QTCONFFILE__PRIVATE__TAG_HPP__INCLUDED
