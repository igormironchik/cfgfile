
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2016 Igor Mironchik

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

// Generator include.
#include "generator.hpp"

// Qt include.
#include <QTextStream>
#include <QStack>

// C++ include.
#include <algorithm>


namespace QtConfFile {

namespace Generator {

//
// Generator
//

Generator::Generator()
{
}

Generator::~Generator()
{
}


typedef const Cfg::Namespace * ConstNamespacePointer;


//
// CppGenerator
//

CppGenerator::CppGenerator( const Cfg::Model & model )
	:	m_model( model )
{
}

CppGenerator::~CppGenerator()
{
}

typedef QStack< ConstNamespacePointer > NamespaceStack;

static inline void closeNamespace( QTextStream & stream,
	NamespaceStack & stack )
{
	stream << QLatin1String( "} /* " )
		<< stack.top()->name()
		<< QLatin1String( " */\n\n" );

	stack.pop();
}

static inline void generateIncludes( QTextStream & stream,
	const QStringList & globalIncludes,
	const QStringList & relativeIncludes )
{
	stream << QLatin1String( "// QtConfFile include."
							 "#include <QtConfFile/TagNoValue>\n"
							 "#include <QtConfFile/TagScalar>\n"
							 "#include <QtConfFile/TagScalarVector>\n"
							 "#include <QtConfFile/TagVectorOfTags>\n"
							 "#include <QtConfFile/ConstraintMinMax>\n"
							 "#include <QtConfFile/ConstraintOneOf>\n"
							 "#include <QtConfFile/Exceptions>\n"
							 "#include <QtConfFile/Utils>\n\n" );

	foreach( const QString & incl, globalIncludes )
		stream << QLatin1String( "#include <" )
			<< incl << QLatin1String( ">\n" );

	if( !globalIncludes.isEmpty() )
		stream << QLatin1String( "\n" );

	foreach( const QString & incl, relativeIncludes )
		stream << QLatin1String( "#include \"" )
			<< incl << QLatin1String( "\"\n" );

	if( !relativeIncludes.isEmpty() )
		stream << QLatin1String( "\n" );
}

static inline void generateCppClasses( QTextStream & stream,
	Cfg::ConstClassPointer c )
{

}

void
CppGenerator::generate( QTextStream & stream ) const
{
	quint64 index = 0;

	NamespaceStack nms;

	Cfg::ConstClassPointer c = 0;

	const QString guard = m_model.includeGuard() + QLatin1String( "_INCLUDED" );

	stream << QLatin1String( "\n#ifndef " ) << guard
		<< QLatin1String( "\n#define " ) << guard
		<< QLatin1String( "\n\n" );

	generateIncludes( stream, m_model.globalIncludes(),
		m_model.relativeIncludes() );

	while( ( c = m_model.nextClass( index ) ) )
	{
		++index;

		NamespaceStack tmp;

		ConstNamespacePointer n = c->parentNamespace();

		while( n )
		{
			if( !n->name().isEmpty() )
				tmp.push( n );

			n = n->parentNamespace();
		}

		if( !tmp.isEmpty() )
		{
			std::reverse( tmp.begin(), tmp.end() );

			while( !nms.isEmpty() && nms.top() != tmp.top() )
				closeNamespace( stream, nms );

			if( nms.isEmpty() )
				nms.swap( tmp );
		}
		else
		{
			while( !nms.isEmpty() )
				closeNamespace( stream, nms );
		}

		generateCppClasses( stream, c );
	}

	while( !nms.isEmpty() )
		closeNamespace( stream, nms );

	stream << QLatin1String( "#endif // " ) << guard
		<< QLatin1String( "\n" );
}

} /* namespace Generator */

} /* namespace QtConfFile */
