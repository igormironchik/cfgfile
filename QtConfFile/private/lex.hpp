
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

#ifndef QTCONFFILE__PRIVATE__LEX_HPP__INCLUDED
#define QTCONFFILE__PRIVATE__LEX_HPP__INCLUDED

// Qt include.
#include <QtCore/QScopedPointer>
#include <QtCore/QString>


namespace QtConfFile {

class InputStream;

//
// LexemeType
//

//! Type of the lexeme.
enum LexemeType {
    //! Null lexeme.
    NullLexeme,
    //! Start tag lexeme "{".
    StartTagLexeme,
    //! Finish tag lexeme "}".
    FinishTagLexeme,
    //! String lexeme.
    StringLexeme
}; // enum LexemeType


//
// Lexeme
//

//! Lexeme.
class Lexeme {
public:
	Lexeme();
    Lexeme( LexemeType type, const QString & value );

	Lexeme & operator = ( const Lexeme & other );

    //! \return Lexeme type.
    LexemeType type() const;

    //! \return Lexeme value.
    const QString & value() const;

    //! \return Is lexeme a null lexeme.
    bool isNull() const;

private:
    //! Lexeme type.
    LexemeType m_type;
    //! Lexeme value.
    QString m_value;
}; // struct Lexeme


//
// LexicalAnalyzer
//

//! Lexicographical analyzer.
class LexicalAnalyzer {
public:
	explicit LexicalAnalyzer( InputStream & stream );
	~LexicalAnalyzer();

	/*!
		\return Next lexeme.

		\throw Exception on lexicographical error.
	*/
	Lexeme nextLexeme();

    //! \return Input stream.
    InputStream & inputStream();

	//! \return Line number.
	qint64 lineNumber() const;

	//! \return Column number.
	qint64 columnNumber() const;

private:
	Q_DISABLE_COPY( LexicalAnalyzer )

    class LexicalAnalyzerPrivate;
	QScopedPointer< LexicalAnalyzerPrivate > d;
}; // class LexicalAnalyzer

} /* namespace QtConfFile */

#endif // QTCONFFILE__PRIVATE__LEX_HPP__INCLUDED
