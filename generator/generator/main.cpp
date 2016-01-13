
// Qt include.
#include <QCoreApplication>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>

// QtArg include.
#include <QtArg/Arg>
#include <QtArg/Help>
#include <QtArg/CmdLine>
#include <QtArg/Exceptions>

// QtConfFile include.
#include <QtConfFile/Utils>

// Generator include.
#include "generator.hpp"


//
// ForGeneration
//

//! Data uses to generate.
class ForGeneration {
public:
	ForGeneration()
	{
	}

	ForGeneration( const QString & inputFile, const QString & outputFile )
		:	m_inputFileName( inputFile )
		,	m_outputFileName( outputFile )
	{
	}

	~ForGeneration()
	{
	}

	ForGeneration( const ForGeneration & other )
		:	m_inputFileName( other.inputFile() )
		,	m_outputFileName( other.outputFile() )
	{
	}

	ForGeneration & operator = ( const ForGeneration & other )
	{
		if( this != &other )
		{
			m_inputFileName = other.inputFile();
			m_outputFileName = other.outputFile();
		}

		return *this;
	}

	//! \return Input file name.
	const QString & inputFile() const
	{
		return m_inputFileName;
	}

	//! \return Output file name.
	const QString & outputFile() const
	{
		return m_outputFileName;
	}

private:
	//! Input file name.
	QString m_inputFileName;
	//! Output file name.
	QString m_outputFileName;
}; // class ForGeneration


//
// parseCommandLineArguments
//

static inline ForGeneration parseCommandLineArguments( int argc, char ** argv )
{
	QtArg input( QLatin1Char( 'i' ), QLatin1String( "input" ),
		QLatin1String( "Input file name" ), true, true );
	QtArg output( QLatin1Char( 'o' ), QLatin1String( "output" ),
		QLatin1String( "Output file name" ), true, true );

	QtArgCmdLine cmd( argc, argv );

	QtArgHelp help( &cmd );
	help.printer()->setProgramDescription(
		QLatin1String( "C++ header generator for QtConfFile." ) );
	help.printer()->setExecutableName( QLatin1String( argv[ 0 ] ) );

	cmd.addArg( &input );
	cmd.addArg( &output );
	cmd.addArg( &help );

	cmd.parse();

	ForGeneration data( input.value().toString(),
		output.value().toString() );

	QTextStream stream( stdout );

	if( data.inputFile().isEmpty() )
	{
		stream << QLatin1String( "Please specify input file." );

		exit( 1 );
	}

	if( !QFileInfo( data.inputFile() ).exists() )
	{
		stream << QLatin1String( "Specified input file doesn't exist." );

		exit( 1 );
	}

	if( data.outputFile().isEmpty() )
	{
		stream << QLatin1String( "Please specify output file." );

		exit( 1 );
	}

	return data;
}


int main( int argc, char ** argv )
{	
	ForGeneration data;

	try {
		data = parseCommandLineArguments( argc, argv );
	}
	catch( const QtArgHelpHasPrintedEx & x )
	{
		return 0;
	}
	catch( const QtArgBaseException & x )
	{
		QTextStream stream( stdout );

		stream << x.whatAsQString();

		return 1;
	}

	QtConfFile::Generator::Cfg::Model model;

	try {
		QtConfFile::Generator::Cfg::TagModel tag;

		QtConfFile::readQtConfFile( tag, data.inputFile(),
			QTextCodec::codecForName( "UTF-8" ) );

		model = tag.cfg();

		model.prepare();

		model.check();
	}
	catch( const QtConfFile::Exception & x )
	{
		QTextStream stream( stdout );

		stream << x.whatAsQString();

		return 1;
	}

	QFile output( data.outputFile() );

	if( output.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
	{
		QTextStream stream( &output );

		QtConfFile::Generator::CppGenerator gen( model );

		gen.generate( stream );

		output.close();

		return 0;
	}
	else
	{
		QTextStream stream( stdout );

		stream << QLatin1String( "Couldn't open output file for writting." );

		return 1;
	}
}
