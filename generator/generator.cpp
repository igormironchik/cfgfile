
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

// generator_t include.
#include "generator.hpp"

// Qt include.
#include <QTextStream>
#include <QStack>

// C++ include.
#include <algorithm>


namespace cfgfile {

namespace generator_t {

//
// generator_t
//

generator_t::generator_t()
{
}

generator_t::~generator_t()
{
}


typedef const Cfg::Namespace * ConstNamespacePointer;


//
// cpp_generator_t
//

cpp_generator_t::cpp_generator_t( const Cfg::Model & model )
	:	m_model( model )
{
}

cpp_generator_t::~cpp_generator_t()
{
}


//
// NamespaceStack
//

typedef QStack< ConstNamespacePointer > NamespaceStack;


//
// startNamespaces
//

static inline void startNamespaces( QTextStream & stream,
	NamespaceStack & stack )
{
	foreach( const ConstNamespacePointer & n, stack )
		stream << QLatin1String( "namespace " )
			<< n->name()
			<< QLatin1String( " {\n\n" );
} // closeNamespaces


//
// closeNamespace
//

static inline void closeNamespace( QTextStream & stream,
	NamespaceStack & stack )
{
	stream << QLatin1String( "} /* namespace " )
		<< stack.top()->name()
		<< QLatin1String( " */\n\n" );

	stack.pop();
} // closeNamespace


//
// generateIncludes
//

static inline void generateIncludes( QTextStream & stream,
	const QStringList & globalIncludes,
	const QStringList & relativeIncludes )
{
	stream << QLatin1String( "// cfgfile include.\n"
							 "#include <cfgfile/tag_no_value_t>\n"
							 "#include <cfgfile/tag_scalar_t>\n"
							 "#include <cfgfile/tag_scalar_tVector>\n"
							 "#include <cfgfile/tag_vector_of_tags_t>\n"
							 "#include <cfgfile/ConstraintMinMax>\n"
							 "#include <cfgfile/constraint_one_of_t>\n"
							 "#include <cfgfile/Exceptions>\n"
							 "#include <cfgfile/Utils>\n\n" );

	stream << QLatin1String( "// Qt include.\n" )
		<< QLatin1String( "#include <std::list>\n\n" );

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
} // generateIncludes


//
// generateSetterMethodName
//

static inline QString generateSetterMethodName( const QString & name )
{
	return ( QString( "set" ) + name.at( 0 ).toUpper() +
		name.right( name.length() - 1 ) );
} // generateSetterMethodName


//
// generatetag_class_tName
//

static inline QString generatetag_class_tName( const QString & name )
{
	const int pos = name.lastIndexOf( Cfg::c_namespace_separator );

	QString res = ( pos == -1 ? QString() :
		name.left( pos + Cfg::c_namespace_separator.length() ) );
	res.append( QLatin1String( "Tag" ) );
	res.append( ( pos == -1 ? name :
		name.right( name.length() - pos -
			Cfg::c_namespace_separator.length() ) ) );

	return res;
}


//
// generateBasetag_class_tName
//

static inline QString generateBasetag_class_tName( const QString & base,
	const QString & value_type )
{
	if( base == Cfg::c_scalar_tag_name )
		return QString( "cfgfile::tag_scalar_t< " ) + value_type +
			QLatin1String( " >" );
	else if( base == Cfg::c_no_value_tag_name )
		return QString( "cfgfile::tag_no_value_t" );
	else if( base == Cfg::c_scalar_vector_tag_name )
		return QString( "cfgfile::tag_scalar_tVector< " ) + value_type +
				QLatin1String( " >" );
	else if( base == Cfg::c_vector_of_tags_tag_name )
		return QString( "cfgfile::tag_vector_of_tags_t< " ) + value_type +
				QLatin1String( " >" );
	else
		return generatetag_class_tName( base );
} // generateBasetag_class_tName


//
// generateTagNameFromClassName
//

static inline QString generateTagNameFromClassName( const QString & name )
{
	return ( name.at( 0 ).toLower() +
		name.right( name.length() - 1 ) );
} // generateTagNameFromClassName


//
// boolToString
//

static inline QString boolToString( bool value )
{
	if( value )
		return QLatin1String( "true" );
	else
		return QLatin1String( "false" );
} // boolToString


//
// generatetag_field_tsInCtor
//

static inline void generatetag_field_tsInCtor( QTextStream & stream,
	Cfg::ConstClassPointer c )
{
	foreach( const Cfg::Field & f, c->fields() )
	{
		if( !f.isBase() )
		{
			stream << QLatin1String( "\t\t,\tm_" )
				<< f.name() << QLatin1String( "( *this, QLatin1String( \"" )
				<< f.name() << QLatin1String( "\" ), " )
				<< boolToString( f.isRequired() )
				<< QLatin1String( " )\n" );

			if( !f.isConstraintNull() )
			{
				Cfg::constraint_base_t * constr = f.constraint().data();

				switch( constr->type() )
				{
					case Cfg::constraint_base_t::min_max_constraint_type :
					{
						Cfg::min_max_constraint_t * minMax =
							static_cast< Cfg::min_max_constraint_t* > ( constr );

						stream << QLatin1String( "\t\t,\tm_" )
							<< f.name() << QLatin1String( "Constraint( " )
							<< minMax->min() << QLatin1String( ", " )
							<< minMax->max() << QLatin1String( " )\n" );
					}
						break;

					case Cfg::constraint_base_t::one_of_constraint_type :
					{
						stream << QLatin1String( "\t\t,\tm_" )
							<< f.name() << QLatin1String( "Constraint()\n" );
					}
						break;

					default :
						break;
				}
			}
		}
	}
} // generatetag_field_tsInCtor


//
// generateConstraintsInCtor
//

static inline void generateConstraintsInCtor( QTextStream & stream,
	Cfg::ConstClassPointer c )
{
	foreach( const Cfg::Field & f, c->fields() )
	{
		if( !f.isConstraintNull() )
		{
			Cfg::constraint_base_t * constr = f.constraint().data();

			switch( constr->type() )
			{
				case Cfg::constraint_base_t::min_max_constraint_type :
				{
					if( !f.isBase() )
						stream << QLatin1String( "\t\tm_" )
							<< f.name() << QLatin1String( ".setConstraint( &m_" )
							<< f.name() << QLatin1String( "Constraint );\n\n" );
					else
						stream << QLatin1String( "\t\tsetConstraint( &m_" )
							<< f.name() << QLatin1String( "Constraint );\n\n" );
				}
					break;

				case Cfg::constraint_base_t::one_of_constraint_type :
				{
					Cfg::one_of_constraint_t * oneOf =
						static_cast< Cfg::one_of_constraint_t* > ( constr );

					foreach( const QString & s, oneOf->values() )
					{
						stream << QLatin1String( "\t\tm_" )
							<< f.name()
							<< QLatin1String( "Constraint.addValue( " )
							<< s << QLatin1String( " );\n" );
					}

					if( !f.isBase() )
						stream << QLatin1String( "\t\tm_" )
							<< f.name() << QLatin1String( ".setConstraint( &m_" )
							<< f.name() << QLatin1String( "Constraint );\n\n" );
					else
						stream << QLatin1String( "\t\tsetConstraint( &m_" )
							<< f.name() << QLatin1String( "Constraint );\n\n" );
				}
					break;

				default :
					break;
			}
		}
	}
} // generateConstraintsInCtor


//
// generateTypeOfData
//

static inline QString generateTypeOfData( const Cfg::Field & f )
{
	switch( f.type() )
	{
		case Cfg::Field::NoValuefield_type_t :
			return QLatin1String( "bool" );

		case Cfg::Field::CustomTagfield_type_t :
		case Cfg::Field::Scalarfield_type_t :
			return f.value_type();

		case Cfg::Field::VectorOfTagsfield_type_t :
		case Cfg::Field::ScalarVectorfield_type_t :
			return QString( "std::list< " ) + f.value_type() + QLatin1String( " >" );

		default :
			return QLatin1String( "void" );
	}
} // generateTypeOfData


//
// generateDataClass
//

static inline void generateDataClass( QTextStream & stream,
	Cfg::ConstClassPointer c )
{
	stream << QLatin1String( "//\n// " )
		<< c->name() << QLatin1String( "\n//\n\n" );

	stream << QLatin1String( "class " ) << c->name() << QLatin1String( " {\n" );
	stream << QLatin1String( "public:\n" );

	// c_tor and d_tor.
	stream << QLatin1String( "\t" ) << c->name()
		<< QLatin1String( "()\n" );

	int i = 0;

	foreach( const Cfg::Field & f, c->fields() )
	{
		if( !f.defaultValue().isEmpty() )
		{
			if( i == 0 )
				stream << QLatin1String( "\t\t:" );
			else
				stream << QLatin1String( "\t\t," );

			stream << QLatin1String( "\tm_" )
				<< f.name() << QLatin1String( "( " )
				<< f.defaultValue() << QLatin1String( " )\n" );

			++i;
		}
	}

	stream << QLatin1String( "\t{\n"
							 "\t}\n"
							 "\t~" )
		<< c->name() << QLatin1String( "()\n"
									   "\t{\n"
									   "\t}\n\n" );

	// Copy c_tor.
	stream << QLatin1String( "\t" ) << c->name()
		<< QLatin1String( "( const " ) << c->name()
		<< QLatin1String( " & other )\n" );

	i = 0;

	foreach( const Cfg::Field & f, c->fields() )
	{
		if( i == 0 )
			stream << QLatin1String( "\t\t:" );
		else
			stream << QLatin1String( "\t\t," );

		stream << QLatin1String( "\tm_" )
			<< f.name() << QLatin1String( "( other." )
			<< f.name() << QLatin1String( "() )\n" );

		++i;
	}

	stream << QLatin1String( "\t{\n"
							 "\t}\n" );

	// operator =.
	stream << QLatin1String( "\t" ) << c->name()
		<< QLatin1String( " & operator = ( const " ) << c->name()
		<< QLatin1String( " & other )\n"
						  "\t{\n"
						  "\t\tif( this != &other )\n"
						  "\t\t{\n" );

	foreach( const Cfg::Field & f, c->fields() )
	{
		stream << QLatin1String( "\t\t\tm_" )
			<< f.name() << QLatin1String( " = other." )
			<< f.name() << QLatin1String( "();\n" );
	}

	stream << QLatin1String( "\t\t}\n\n"
							 "\t\treturn *this;\n"
							 "\t}\n\n" );

	// Getters and setters.
	foreach( const Cfg::Field & f, c->fields() )
	{
		if( f.type() != Cfg::Field::NoValuefield_type_t )
		{
			stream << QLatin1String( "\tconst " )
				<< generateTypeOfData( f ) << QLatin1String( " & " )
				<< f.name() << QLatin1String( "() const\n"
											  "\t{\n"
											  "\t\treturn m_" )
				<< f.name() << QLatin1String( ";\n"
											  "\t}\n" );

			stream << QLatin1String( "\t" )
				<< generateTypeOfData( f ) << QLatin1String( " & " )
				<< f.name() << QLatin1String( "()\n"
											  "\t{\n"
											  "\t\treturn m_" )
				<< f.name() << QLatin1String( ";\n"
											  "\t}\n" );

			stream << QLatin1String( "\tvoid " )
				<< generateSetterMethodName( f.name() )
				<< QLatin1String( "( const " ) << generateTypeOfData( f )
				<< QLatin1String( " & v )\n"
								  "\t{\n"
								  "\t\tm_" ) << f.name()
				<< QLatin1String( " = v;\n"
								  "\t}\n\n" );
		}
		else
		{
			stream << QLatin1String( "\tbool " )
				<< f.name() << QLatin1String( "() const\n"
											  "\t{\n"
											  "\t\treturn m_" )
				<< f.name() << QLatin1String( ";\n"
											  "\t}\n" );

			stream << QLatin1String( "\tvoid " )
				<< generateSetterMethodName( f.name() )
				<< QLatin1String( "( bool v = true )\n"
								  "\t{\n"
								  "\t\tm_" ) << f.name()
				<< QLatin1String( " = v;\n"
								  "\t}\n\n" );
		}
	}

	// Private members.
	stream << QLatin1String( "private:\n" );

	foreach( const Cfg::Field & f, c->fields() )
	{
		stream << QLatin1String( "\t" ) << generateTypeOfData( f )
			<< QLatin1String( " m_" ) << f.name()
			<< QLatin1String( ";\n" );
	}

	stream << QLatin1String( "}; // class " ) << c->name()
		<< QLatin1String( "\n\n\n" );
} // generateDataClass


//
// generateCfgInit
//

static inline void generateCfgInit( QTextStream & stream,
	Cfg::ConstClassPointer c )
{
	foreach( const Cfg::Field & f, c->fields() )
	{
		if( !f.isBase() )
		{
			switch( f.type() )
			{
				case Cfg::Field::NoValuefield_type_t :
				{
					stream << QLatin1String( "\n\t\tif( m_" )
						<< f.name() << QLatin1String( ".isDefined() )\n"
													  "\t\t\tc." )
						<< generateSetterMethodName( f.name() )
						<< QLatin1String( "( true );\n\n" );
				}
					break;

				case Cfg::Field::Scalarfield_type_t :
				{
					stream << QLatin1String( "\n\t\tif( m_" )
						<< f.name() << QLatin1String( ".isDefined() )\n"
								"\t\t\tc." )
						<< generateSetterMethodName( f.name() )
						<< QLatin1String( "( m_" )
						<< f.name() << QLatin1String( ".value() );\n" );
				}
					break;

				case Cfg::Field::ScalarVectorfield_type_t :
				{
					stream << QLatin1String( "\n\t\tif( m_" )
						<< f.name() << QLatin1String( ".isDefined() )\n"
								"\t\t\tc." )
						<< generateSetterMethodName( f.name() )
						<< QLatin1String( "( m_" )
						<< f.name() << QLatin1String( ".values().toList() );\n" );
				}
					break;

				case Cfg::Field::VectorOfTagsfield_type_t :
				{
					stream << QLatin1String( "\n\t\tif( m_" )
						   << f.name() << QLatin1String( ".isDefined() )\n"
								   "\t\t{\n" )
						<< QLatin1String( "\t\t\tstd::list< " )
						<< f.value_type() << QLatin1String( " > " )
						<< f.name() << QLatin1String( "_" )
						<< generateTagNameFromClassName( f.value_type() )
						<< QLatin1String( "List;\n\n" )
						<< QLatin1String( "\t\t\tfor( int i = 0; i < m_" )
						<< f.name() << QLatin1String( ".size(); ++i )\n" )
						<< QLatin1String( "\t\t\t\t" )
						<< f.name() << QLatin1String( "_" )
						<< generateTagNameFromClassName( f.value_type() )
						<< QLatin1String( "List.append( m_" ) << f.name()
						<< QLatin1String( ".at( i ).getCfg() );\n\n" )
						<< QLatin1String( "\t\t\tc." )
						<< generateSetterMethodName( f.name() )
						<< QLatin1String( "( " )
						<< f.name() << QLatin1String( "_" )
						<< generateTagNameFromClassName( f.value_type() )
						<< QLatin1String( "List );\n" )
						<< QLatin1String( "\t\t}\n" );
				}
					break;

				case Cfg::Field::CustomTagfield_type_t :
				{
					stream << QLatin1String( "\n\t\tif( m_" )
						<< f.name() << QLatin1String( ".isDefined() )\n"
								"\t\t\tc." )
						<< generateSetterMethodName( f.name() )
						<< QLatin1String( "( m_" )
						<< f.name() << QLatin1String( ".getCfg() );\n" );
				}
					break;

				default :
					break;
			}
		}
		else
		{
			switch( f.type() )
			{
				case Cfg::Field::Scalarfield_type_t :
				{
					stream << QLatin1String( "\t\tc." )
						<< generateSetterMethodName( f.name() )
						<< QLatin1String( "( value() );\n" );
				}
					break;

				case Cfg::Field::ScalarVectorfield_type_t :
				{
					stream << QLatin1String( "\t\tc." )
						<< generateSetterMethodName( f.name() )
						<< QLatin1String( "( values().toList() );\n" );
				}
					break;

				default :
					break;
			}
		}
	}
} // generateCfgInit


//
// generateCfgSet
//

static inline void generateCfgSet( QTextStream & stream,
	Cfg::ConstClassPointer c )
{
	foreach( const Cfg::Field & f, c->fields() )
	{
		if( !f.isBase() )
		{
			switch( f.type() )
			{
				case Cfg::Field::NoValuefield_type_t :
				{
					stream << QLatin1String( "\n\t\tif( cfg." )
						<< f.name() << QLatin1String( "() )\n"
													  "\t\t\tm_" )
						<< f.name() << QLatin1String( ".setDefined();\n\n" );
				}
					break;

				case Cfg::Field::Scalarfield_type_t :
				{
					stream << QLatin1String( "\t\tm_" )
						<< f.name()
						<< QLatin1String( ".setValue( cfg." )
						<< f.name() << QLatin1String( "() );\n" );
				}
					break;

				case Cfg::Field::ScalarVectorfield_type_t :
				{
					stream << QLatin1String( "\t\tm_" )
						<< f.name() << QLatin1String( ".setValues( cfg." )
						<< f.name() << QLatin1String( "().toVector() );\n" );
				}
					break;

				case Cfg::Field::VectorOfTagsfield_type_t :
				{
					stream << QLatin1String( "\n\t\tforeach( const " )
						<< f.value_type() << QLatin1String( " & v, cfg." )
						<< f.name() << QLatin1String( "() )\n" )
						<< QLatin1String( "\t\t{\n" )
						<< QLatin1String( "\t\t\tcfgfile::tag_vector_of_tags_t< " )
						<< generatetag_class_tName( f.value_type() )
						<< QLatin1String( " >::PointerToTag p(\n" )
						<< QLatin1String( "\t\t\t\tnew " )
						<< generatetag_class_tName( f.value_type() )
						<< QLatin1String( "( \"" )
						<< f.name() << QLatin1String( "\", " )
						<< boolToString( f.isRequired() )
						<< QLatin1String( " ) );\n\n" )
						<< QLatin1String( "\t\t\tp->setCfg( v );\n\n" )
						<< QLatin1String( "\t\t\tm_" ) << f.name()
						<< QLatin1String( ".setValue( p );\n" )
						<< QLatin1String( "\t\t}\n" );
				}
					break;

				case Cfg::Field::CustomTagfield_type_t :
				{
					stream << QLatin1String( "\t\tm_" )
						<< f.name()
						<< QLatin1String( ".setCfg( cfg." )
						<< f.name() << QLatin1String( "() );\n" );
				}
					break;

				default :
					break;
			}
		}
		else
		{
			switch( f.type() )
			{
				case Cfg::Field::Scalarfield_type_t :
				{
					stream << QLatin1String( "\t\tsetValue( cfg." )
						<< f.name() << QLatin1String( "() );\n" );
				}
					break;

				case Cfg::Field::ScalarVectorfield_type_t :
				{
					stream << QLatin1String( "\t\tsetValues( cfg." )
						<< f.name() << QLatin1String( "().toVector() );\n" );
				}
					break;

				default :
					break;
			}
		}
	}

	stream << QLatin1String( "\n\t\tsetDefined();\n" );
} // generateCfgSet


//
// generatePrivateTagMembers
//

static inline void generatePrivateTagMembers( QTextStream & stream,
	Cfg::ConstClassPointer c )
{
	foreach( const Cfg::Field & f, c->fields() )
	{
		if( !f.isBase() )
		{
			switch( f.type() )
			{
				case Cfg::Field::NoValuefield_type_t :
				{
					stream << QLatin1String( "\tcfgfile::tag_no_value_t m_" )
						<< f.name() << QLatin1String( ";\n" );
				}
					break;

				case Cfg::Field::Scalarfield_type_t :
				{
					stream << QLatin1String( "\tcfgfile::tag_scalar_t< " )
						<< f.value_type() << QLatin1String( " > m_" )
						<< f.name() << QLatin1String( ";\n" );
				}
					break;

				case Cfg::Field::ScalarVectorfield_type_t :
				{
					stream << QLatin1String( "\tcfgfile::tag_scalar_tVector< " )
						<< f.value_type() << QLatin1String( " > m_" )
						<< f.name() << QLatin1String( ";\n" );
				}
					break;

				case Cfg::Field::VectorOfTagsfield_type_t :
				{
					stream << QLatin1String( "\tcfgfile::tag_vector_of_tags_t< " )
						<< generatetag_class_tName( f.value_type() )
						<< QLatin1String( " > m_" )
						<< f.name() << QLatin1String( ";\n" );
				}
					break;

				case Cfg::Field::CustomTagfield_type_t :
				{
					stream << QLatin1String( "\t" )
						<< generatetag_class_tName( f.value_type() )
						<< QLatin1String( " m_" )
						<< f.name() << QLatin1String( ";\n" );
				}
					break;

				default :
					break;
			}
		}

		if( !f.isConstraintNull() )
		{
			Cfg::constraint_base_t * constr = f.constraint().data();

			switch( constr->type() )
			{
				case Cfg::constraint_base_t::min_max_constraint_type :
				{
					stream << QLatin1String( "\tcfgfile::ConstraintMinMax< " )
						<< f.value_type() << QLatin1String( " > m_" )
						<< f.name() << QLatin1String( "Constraint;\n" );
				}
					break;

				case Cfg::constraint_base_t::one_of_constraint_type :
				{
					stream << QLatin1String( "\tcfgfile::constraint_one_of_t< " )
						<< f.value_type() << QLatin1String( " > m_" )
						<< f.name() << QLatin1String( "Constraint;\n" );
				}
					break;

				default :
					break;
			}
		}
	}
} // generatePrivateTagMembers


//
// generatetag_class_t
//

static inline void generatetag_class_t( QTextStream & stream,
	Cfg::ConstClassPointer c )
{
	const QString tagClassName = QString( "Tag" ) + c->name();

	stream << QLatin1String( "//\n"
							 "// " )
		<< tagClassName << QLatin1String( "\n"
									   "//\n\n" );

	stream << QLatin1String( "class " ) << tagClassName
		<< QLatin1String( "\n"
						  "\t:\tpublic " );

	const QString baseTag = generateBasetag_class_tName( c->baseName(),
		c->baseValueType() );

	stream << baseTag << QLatin1String( "\n" );
	stream << QLatin1String( "{\n" );
	stream << QLatin1String( "public:\n" );

	// c_tors.
	// 1
	const QString tagName = generateTagNameFromClassName( c->name() );

	stream << QLatin1String( "\t" ) << tagClassName
		<< QLatin1String( "()\n"
						  "\t\t:\t" )
		<< baseTag << QLatin1String( "( QLatin1String( \"" )
		<< tagName << QLatin1String( "\" ), true )\n" );

	generatetag_field_tsInCtor( stream, c );

	stream << QLatin1String( "\t{\n" );

	generateConstraintsInCtor( stream, c );

	stream << QLatin1String( "\t}\n\n" );

	// 2
	stream << QLatin1String( "\texplicit " ) << tagClassName
		<< QLatin1String( "( const " ) << c->name()
		<< QLatin1String( " & cfg )\n"
						  "\t\t:\t" )
		<< baseTag << QLatin1String( "( QLatin1String( \"" )
		<< tagName << QLatin1String( "\" ), true )\n" );

	generatetag_field_tsInCtor( stream, c );

	stream << QLatin1String( "\t{\n" );

	generateConstraintsInCtor( stream, c );

	stream << QLatin1String( "\t\tsetCfg( cfg );\n" );

	stream << QLatin1String( "\t}\n\n" );

	// 3
	stream << QLatin1String( "\t" ) << tagClassName
		<< QLatin1String( "( const QString & name, bool is_mandatory )\n" )
		<< QLatin1String( "\t\t:\t" )
		<< baseTag << QLatin1String( "( name, is_mandatory )\n" );

	generatetag_field_tsInCtor( stream, c );

	stream << QLatin1String( "\t{\n" );

	generateConstraintsInCtor( stream, c );

	stream << QLatin1String( "\t}\n\n" );

	// 4
	stream << QLatin1String( "\t" ) << tagClassName
		<< QLatin1String( "( cfgfile::Tag & owner, const QString & name, "
						  "bool is_mandatory )\n" )
		<< QLatin1String( "\t\t:\t" )
		<< baseTag << QLatin1String( "( owner, name, is_mandatory )\n" );

	generatetag_field_tsInCtor( stream, c );

	stream << QLatin1String( "\t{\n" );

	generateConstraintsInCtor( stream, c );

	stream << QLatin1String( "\t}\n\n" );

	// d_tor.
	stream << QLatin1String( "\t~" ) << tagClassName
		<< QLatin1String( "()\n" )
		<< QLatin1String( "\t{\n"
						  "\t}\n\n" );

	// getter.
	stream << QLatin1String( "\t" ) << c->name()
		<< QLatin1String( " getCfg() const\n"
						  "\t{\n"
						  "\t\t" ) << c->name()
		<< QLatin1String( " c;\n" );

	generateCfgInit( stream, c );

	stream << QLatin1String( "\n\t\treturn c;\n"
							 "\t}\n\n" );

	// setter.
	stream << QLatin1String( "\tvoid setCfg( const " )
		<< c->name() << QLatin1String( " & cfg )\n"
									   "\t{\n" );

	generateCfgSet( stream, c );

	stream << QLatin1String( "\t}\n\n" );

	// private members.
	stream << QLatin1String( "private:\n" );

	generatePrivateTagMembers( stream, c );

	stream << QLatin1String( "}; // class " )
		<< tagClassName << QLatin1String( "\n\n" );
} // generatetag_class_t


//
// generateCppClasses
//

static inline void generateCppClasses( QTextStream & stream,
	Cfg::ConstClassPointer c )
{	
	generateDataClass( stream, c );

	generatetag_class_t( stream, c );
} // generateCppClasses

void
cpp_generator_t::generate( QTextStream & stream ) const
{
	unsigned long long index = 0;

	NamespaceStack nms;

	Cfg::ConstClassPointer c = 0;

	const QString guard = m_model.includeGuard() + QLatin1String( "__INCLUDED" );

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
			{
				nms.swap( tmp );

				startNamespaces( stream, nms );
			}
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

} /* namespace generator_t */

} /* namespace cfgfile */
