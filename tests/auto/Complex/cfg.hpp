
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef CFGFILE__TEST__COMPLEX__CFG_HPP__INCLUDED
#define CFGFILE__TEST__COMPLEX__CFG_HPP__INCLUDED

// cfgfile include.
#include <cfgfile/all.hpp>

// C++ include.
#include <string>
#include <vector>


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
		Pair( const std::string & stringValue, int intValue );

		std::string m_stringValue;
		int m_intValue;
	}; // struct Pair

	Configuration();

	std::string m_stringValue;
	std::vector< std::string > m_listOfStringValues;
	int m_intValue;
	std::vector< Pair > m_vectorOfTags;
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
	:	public cfgfile::tag_no_value_t<>
{
public:
	//! For reading.
	TagVecOfTags( const std::string & name, bool isMandatory );
	//! For writing.
	explicit TagVecOfTags( const Configuration::Pair & pair );

	//! \return Pair of values.
	Configuration::Pair
	pair() const;

private:
	cfgfile::tag_scalar_t< std::string > m_stringValue;
	cfgfile::tag_scalar_t< int > m_intValue;
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
	:	public cfgfile::tag_no_value_t<>
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
	cfgfile::tag_scalar_t< std::string > m_stringValue;
	cfgfile::constraint_one_of_t< std::string > m_constraintForListOfStringValues;
	cfgfile::tag_scalar_vector_t< std::string > m_listOfStringValues;
	cfgfile::constraint_min_max_t< int > m_constriantForIntValue;
	cfgfile::tag_scalar_t< int > m_intValue;
	cfgfile::tag_vector_of_tags_t< TagVecOfTags > m_vecOfTags;
}; // class TagConfiguration

#endif // CFGFILE__TEST__COMPLEX__CFG_HPP__INCLUDED
