
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Example include.
#include "cfg.hpp"

// C++ include.
#include <fstream>
#include <iostream>


int main( int, char ** )
{
	configuration_t cfg;

	std::ifstream in( "example.cfg" );

	try {
		if( in.good() )
		{
			tag_configuration_t readTag;

			cfgfile::read_cfgfile( readTag, in, "example.cfg" );

			in.close();

			cfg = readTag.configuration();
		}
		else
		{
			std::cout << "Can't open file." << std::endl;

			return 1;
		}
	}
	catch( const cfgfile::exception_t< cfgfile::string_trait_t > & x )
	{
		in.close();

		std::cout << x.desc() << std::endl;

		return 1;
	}

	std::cout << "We've loaded the configuration from file:" << std::endl;
	std::cout << "stringValue: " << cfg.m_stringValue << std::endl;

	if( !cfg.m_listOfStringValues.empty() )
	{
		std::cout << "listOfStringValues: ";

		for( const std::string & str : cfg.m_listOfStringValues )
			std::cout << str << " ";

		std::cout << std::endl;
	}

	std::cout << "intValue: " << cfg.m_intValue << std::endl;

	if( !cfg.m_vectorOfTags.empty() )
	{
		int i = 1;

		std::cout << "vecOfTags:" << std::endl;

		for( const configuration_t::pair_t & pair : cfg.m_vectorOfTags )
		{
			std::cout << "Pair " << i << ":" << std::endl;
			std::cout << "stringValue: " << pair.m_stringValue << std::endl;
			std::cout << "intValue: " << pair.m_intValue << std::endl;
			++i;
		}
	}

	std::cout << std::endl << "And now we will save new configuration to file \"new.cfg\". "
		"Look at it!" << std::endl;

	std::ofstream out( "new.cfg" );

	try {
		if( out.good() )
		{
			tag_configuration_t writeTag( cfg );

			cfgfile::write_cfgfile( writeTag, out );

			out.close();
		}
		else
		{
			std::cout << "Can't open file for writting." << std::endl;

			return 1;
		}
	}
	catch( const cfgfile::exception_t< cfgfile::string_trait_t > & x )
	{
		out.close();

		std::cout << x.desc() << std::endl;

		return 1;
	}

	std::cout << "All have been done!" << std::endl;
}
