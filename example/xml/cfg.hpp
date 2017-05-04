
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

#ifndef QTCONFFILE__EXAMPLE__CFG_HPP__INCLUDED
#define QTCONFFILE__EXAMPLE__CFG_HPP__INCLUDED

// QtConfFile include.
#include <QtConfFile/TagNoValue>
#include <QtConfFile/TagScalar>
#include <QtConfFile/TagScalarVector>
#include <QtConfFile/TagVectorOfTags>
#include <QtConfFile/ConstraintMinMax>
#include <QtConfFile/ConstraintOneOf>

// Qt include.
#include <QtCore/QVector>
#include <QtCore/QString>


//
// Configuration
//

/*!
	Configuration that will be loaded from file and savend into file.

	\verbatim
	{cfg
		{stringValue "string"}
		{listOfStringValues "str1" "str2" "str3"}
		{intValue 100}

		{vecOfTags
			{stringValue "str1"}
			{intValue 10}
		}
		{vecOfTags
			{stringValue "str2"}
			{intValue 20}
		}
	}
	\endverbatim
*/
struct Configuration {
	struct Pair {
		Pair();
		Pair( const QString & stringValue, int intValue );

		Pair & operator = ( const Pair & other );

		QString m_stringValue;
		int m_intValue;
	}; // struct Pair

	Configuration();

	Configuration & operator = ( const Configuration & other );

	QString m_stringValue;
	QVector< QString > m_listOfStringValues;
	int m_intValue;
	QVector< Pair > m_vectorOfTags;
}; // class Configuration


//
// TagVecOfTags
//

/*!
	Tag "vecOfTags" with Configuration::Pair as value.

	This class will be used as template type of QtCOnfFile::TagVectorOfTags:

	\verbatim
	{vecOfTags
		{stringValue "str2"}
		{intValue 20}
	}
	\endverbatim
*/
class TagVecOfTags
	:	public QtConfFile::TagNoValue
{
public:
	//! For reading.
	TagVecOfTags( const QString & name, bool isMandatory );
	//! For writing.
	explicit TagVecOfTags( const Configuration::Pair & pair );

	//! \return Pair of values.
	Configuration::Pair
	pair() const;

private:
	QtConfFile::TagScalar< QString > m_stringValue;
	QtConfFile::TagScalar< int > m_intValue;
}; // class TagVecOfTags


//
// TagConfiguration
//

/*!
	Tag with configuration:

	\verbatim
	{cfg
		{stringValue "string"}
		{listOfStringValues "str1" "str2" "str3"}
		{intValue 100}

		{vecOfTags
			{stringValue "str1"}
			{intValue 10}
		}
		{vecOfTags
			{stringValue "str2"}
			{intValue 20}
		}
	}
	\endverbatim
*/
class TagConfiguration
	:	public QtConfFile::TagNoValue
{
public:
	//! For reading.
	TagConfiguration();
	//! For writing.
	explicit TagConfiguration( const Configuration & cfg );

	//! \return Configuration.
	Configuration
	configuration() const;

private:
	QtConfFile::TagScalar< QString > m_stringValue;
	QtConfFile::ConstraintOneOf< QString > m_constraintForListOfStringValues;
	QtConfFile::TagScalarVector< QString > m_listOfStringValues;
	QtConfFile::ConstraintMinMax< int > m_constriantForIntValue;
	QtConfFile::TagScalar< int > m_intValue;
	QtConfFile::TagVectorOfTags< TagVecOfTags > m_vecOfTags;
}; // class TagConfiguration

#endif // QTCONFFILE__EXAMPLE__CFG_HPP__INCLUDED
