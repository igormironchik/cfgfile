
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef cfgfile__EXAMPLE__CFG_HPP__INCLUDED
#define cfgfile__EXAMPLE__CFG_HPP__INCLUDED

// cfgfile include.
#include <cfgfile/all.hpp>

// C++ include.
#include <vector>


//
// configuration_t
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
struct configuration_t {
	struct pair_t {
		pair_t();
		pair_t( const std::string & stringValue, int intValue );

		std::string m_stringValue;
		int m_intValue;
	}; // struct Pair

	configuration_t();

	std::string m_stringValue;
	std::vector< std::string > m_listOfStringValues;
	int m_intValue;
	std::vector< pair_t > m_vectorOfTags;
}; // class configuration_t


//
// tag_vec_of_tags_t
//

/*!
	Tag "vecOfTags" with Configuration::Pair as value.

	This class will be used as template type of cfgfile::TagVectorOfTags:

	\verbatim
	{vecOfTags
		{stringValue "str2"}
		{intValue 20}
	}
	\endverbatim
*/
class tag_vec_of_tags_t
	:	public cfgfile::tag_no_value_t<>
{
public:
	//! For reading.
	tag_vec_of_tags_t( const std::string & name, bool isMandatory );
	//! For writing.
	explicit tag_vec_of_tags_t( const configuration_t::pair_t & pair );

	//! \return Pair of values.
	configuration_t::pair_t
	pair() const;

private:
	cfgfile::tag_scalar_t< std::string > m_stringValue;
	cfgfile::tag_scalar_t< int > m_intValue;
}; // class tag_vec_of_tags_t


//
// tag_configuration_t
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
class tag_configuration_t
	:	public cfgfile::tag_no_value_t<>
{
public:
	//! For reading.
	tag_configuration_t();
	//! For writing.
	explicit tag_configuration_t( const configuration_t & cfg );

	//! \return Configuration.
	configuration_t
	configuration() const;

private:
	cfgfile::tag_scalar_t< std::string > m_stringValue;
	cfgfile::constraint_one_of_t< std::string > m_constraintForListOfStringValues;
	cfgfile::tag_scalar_vector_t< std::string > m_listOfStringValues;
	cfgfile::constraint_min_max_t< int > m_constriantForIntValue;
	cfgfile::tag_scalar_t< int > m_intValue;
	cfgfile::tag_vector_of_tags_t< tag_vec_of_tags_t > m_vecOfTags;
}; // class TagConfiguration

#endif // cfgfile__EXAMPLE__CFG_HPP__INCLUDED
