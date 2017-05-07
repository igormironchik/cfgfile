
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
	Args::Arg input( 'i', "input",
		"Input file name", true, true );
	Args::Arg output( 'o', "output",
		"Output file name", true, true );

	Args::CmdLine cmd( argc, argv );

	Args::Help help;
	help.setProgramDescription(
		QLatin1String( "C++ header generator for cfgfile." ) );
	help.setExecutableName( QLatin1String( argv[ 0 ] ) );

	cmd.addArg( input );
	cmd.addArg( output );
	cmd.addArg( help );

	cmd.parse();

	for_generation_t data( input.value(), output.value() );

	if( data.input_file().isEmpty() )
	{
		std::cout << "Please specify input file." << endl;

		exit( 1 );
	}

	if( !QFileInfo( data.input_file() ).exists() )
	{
		std::cout << "Specified input file doesn't exist." << endl;

		exit( 1 );
	}

	if( data.output_file().isEmpty() )
	{
		std::cout << "Please specify output file." << endl;

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
	catch( const Args::ArgHelpHasPrintedEx & )
	{
		return 0;
	}
	catch( const Args::ArgBaseException & x )
	{
		std::cout << x.desc() << endl;

		return 1;
	}

	cfgfile::generator::cfg::model_t model;

	try {
		cfgfile::generator::cfg::tag_model_t tag;

		cfgfile::read_cfgfile( tag, data.input_file() );

		model = tag.cfg();

		model.prepare();

		model.check();
	}
	catch( const cfgfile::exception_t & x )
	{
		std::cout << x.desc() << endl;

		return 1;
	}

	QFile output( data.output_file() );

	if( output.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
	{
		QTextStream stream( &output );

		cfgfile::generator_t::cpp_generator_t gen( model );

		gen.generate( stream );

		output.close();

		return 0;
	}
	else
	{
		std::cout << "Couldn't open output file for writting."
			<< endl;

		return 1;
	}
}
