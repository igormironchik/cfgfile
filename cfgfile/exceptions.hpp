
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

} /* namespace cfgfile */

#endif // CFGFILE__EXCEPTIONS_HPP__INCLUDED
