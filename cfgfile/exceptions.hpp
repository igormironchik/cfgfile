
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef CFGFILE__EXCEPTIONS_HPP__INCLUDED
#define CFGFILE__EXCEPTIONS_HPP__INCLUDED

// C++ include.
#include <stdexcept>

// cfgfile include.
#include "types.hpp"


namespace cfgfile {

//
// exception_t
//

//! Exception in the library.
template< typename Trait = string_trait_t >
class exception_t final
	:	public std::logic_error
{
public:
	//! Construct exception.
	explicit exception_t( typename Trait::string_t what )
		:	std::logic_error( "Please use desc() method of the exception." )
		,	m_what( std::move( what ) )
	{
	}

	~exception_t() noexcept
	{
	}

	//! \return Reason of the exception.
	const typename Trait::string_t & desc() const noexcept
	{
		return m_what;
	}

private:
	//! Reason of the exception.
	typename Trait::string_t m_what;
}; // class exception_t


#ifndef CFGFILE_DISABLE_STL

//
// exception_t< string_trait_t >
//

template<>
class exception_t< string_trait_t >
	:	public std::logic_error
{
public:
	//! Construct exception.
	explicit exception_t( string_trait_t::string_t what )
		:	std::logic_error( what )
		,	m_what( std::move( what ) )
	{
	}

	~exception_t() noexcept
	{
	}

	//! \return Reason of the exception.
	const string_trait_t::string_t & desc() const noexcept
	{
		return m_what;
	}

private:
	//! Reason of the exception.
	string_trait_t::string_t m_what;
}; // class exception_t< string_trait_t >

#endif // CFGFILE_DISABLE_STL

} /* namespace cfgfile */

#endif // CFGFILE__EXCEPTIONS_HPP__INCLUDED
