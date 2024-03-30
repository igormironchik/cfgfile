
/*
	SPDX-FileCopyrightText: 2017-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// generator include.
#include "example.hpp"

// C++ include.
#include <fstream>
#include <iostream>
#include <vector>


int main()
{
	generator::b_t b;
	b.set_integer_field( 100 );

	std::vector< generator::a_t > list;
	generator::a_t a;
	a.set_string_field( "string" );
	list.push_back( a );

	b.set_a_vector( list );

	std::ofstream out( "generator.cfg" );

	try {
		generator::tag_b_t< cfgfile::string_trait_t > tag( b );

		if( out.good() )
		{
			cfgfile::write_cfgfile( tag, out );

			out.close();
		}
		else
		{
			std::cout << "Fialed to save configuration. Can't open file to write."
				<< std::endl;

			return 1;
		}
	}
	catch( const cfgfile::exception_t< cfgfile::string_trait_t > & x )
	{
		out.close();

		std::cout << x.desc() << std::endl;

		return 1;
	}

	std::cout << "Configuration saved to \"generator.cfg\".\n";

	return 0;
}
