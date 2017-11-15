
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


// cfgfile include.
#include <cfgfile/utils.hpp>

// generator_t include.
#include "generator.hpp"

// C++ include.
#include <fstream>

// Args include.
#include <Args/all.hpp>


//
// for_generation_t
//

//! Data uses to generate.
class for_generation_t {
public:
	for_generation_t()
	{
	}

	for_generation_t( const std::string & input_file, const std::string & output_file )
		:	m_input_file_name( input_file )
		,	m_output_file_name( output_file )
	{
	}

	~for_generation_t()
	{
	}

	for_generation_t( const for_generation_t & other )
		:	m_input_file_name( other.input_file() )
		,	m_output_file_name( other.output_file() )
	{
	}

	for_generation_t & operator = ( const for_generation_t & other )
	{
		if( this != &other )
		{
			m_input_file_name = other.input_file();
			m_output_file_name = other.output_file();
		}

		return *this;
	}

	//! \return Input file name.
	const std::string & input_file() const
	{
		return m_input_file_name;
	}

	//! \return Output file name.
	const std::string & output_file() const
	{
		return m_output_file_name;
	}

private:
	//! Input file name.
	std::string m_input_file_name;
	//! Output file name.
	std::string m_output_file_name;
}; // class for_generation_t


//
// parse_cli
//

static inline for_generation_t parse_cli( int argc, char ** argv )
{
	Args::CmdLine cmd;

	cmd.addArgWithFlagAndName( 'i', "input", true, true, "Input file name" )
		.addArgWithFlagAndName( 'o', "output", true, true, "Output file name" )
		.addHelp( true, argv[ 0 ], "C++ header generator for cfgfile." );

	cmd.parse( argc, argv );

	for_generation_t data( cmd.value( "-i" ), cmd.value( "-o" ) );

	if( data.input_file().empty() )
	{
		std::cout << "Please specify input file." << std::endl;

		exit( 1 );
	}

	if( data.output_file().empty() )
	{
		std::cout << "Please specify output file." << std::endl;

		exit( 1 );
	}

	return data;
}


int main( int argc, char ** argv )
{	
	for_generation_t data;

	try {
		data = parse_cli( argc, argv );
	}
	catch( const Args::HelpHasBeenPrintedException & )
	{
		return 0;
	}
	catch( const Args::BaseException & x )
	{
		std::cout << x.desc() << std::endl;

		return 1;
	}

	std::ifstream in( data.input_file() );

	if( !in.good() )
	{
		std::cout << "Specified input file doesn't exist." << std::endl;

		exit( 1 );
	}

	cfgfile::generator::cfg::model_t model;

	try {
		cfgfile::generator::cfg::tag_model_t tag;

		cfgfile::read_cfgfile( tag, in, data.input_file() );

		in.close();

		model = tag.cfg();

		model.prepare();

		model.check();
	}
	catch( const cfgfile::exception_t< cfgfile::string_trait_t > & x )
	{
		in.close();

		std::cout << x.desc() << std::endl;

		return 1;
	}

	std::ofstream out( data.output_file() );

	if( out.good() )
	{
		cfgfile::generator::cpp_generator_t gen( model );

		gen.generate( out );

		out.close();

		return 0;
	}
	else
	{
		std::cout << "Couldn't open output file for writting."
			<< std::endl;

		return 1;
	}
}
