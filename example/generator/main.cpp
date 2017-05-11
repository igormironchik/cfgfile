
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
