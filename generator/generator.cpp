
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

// C++ include.
#include <algorithm>


namespace cfgfile {

namespace generator {

//
// generator_t
//

generator_t::generator_t()
{
}

generator_t::~generator_t()
{
}


typedef const cfg::namespace_t* const_namespace_ptr_t;


//
// cpp_generator_t
//

cpp_generator_t::cpp_generator_t( const cfg::model_t & model )
	:	m_model( model )
{
}

cpp_generator_t::~cpp_generator_t()
{
}


//
// namespace_stack_t
//

typedef std::list< const_namespace_ptr_t > namespace_stack_t;


//
// startNamespaces
//

static inline void start_namespaces( std::ostream & stream,
	namespace_stack_t & stack )
{
	for( const const_namespace_ptr_t & n : stack )
		stream << "namespace "
			<< n->name()
			<< " {\n\n";
} // closeNamespaces


//
// close_namespace
//

static inline void close_namespace( std::ostream & stream,
	namespace_stack_t & stack )
{
	stream << "} /* namespace "
		<< stack.back()->name()
		<< " */\n\n";

	stack.pop_back();
} // closeNamespace


//
// generate_includes
//

static inline void generate_includes( std::ostream & stream,
	const std::list< std::string > & global_includes,
	const std::list< std::string > & relative_includes )
{
	stream << "// cfgfile include.\n"
		"#include <cfgfile/all.hpp";

	stream << "// C++ include.\n"
		<< "#include <list>\n\n";

	for( const std::string & incl : global_includes )
		stream << "#include <" << incl << ">\n";

	if( !global_includes.empty() )
		stream << "\n";

	for( const std::string & incl : relative_includes )
		stream << "#include \"" << incl << "\"\n";

	if( !relative_includes.empty() )
		stream << "\n";
} // generate_includes


//
// generate_setter_method_name
//

static inline std::string generate_setter_method_name( const std::string & name )
{
	return ( std::string( "set_" ) + name );
} // generate_setter_method_name


//
// generate_class_name
//

static inline std::string generate_class_name( const std::string & name )
{
	const auto pos = name.rfind( cfg::c_namespace_separator );

	std::string res = ( pos == std::string::npos ? std::string() :
		name.substr( 0, pos + cfg::c_namespace_separator.length() ) );
	res.append( "tag_" );
	res.append( ( pos == std::string::npos ? name :
		name.substr( pos + cfg::c_namespace_separator.length() ) ) );
	res.append( "_t" );

	return res;
}


//
// generate_base_class_name
//

static inline std::string generate_base_class_name( const std::string & base,
	const std::string & value_type )
{
	if( base == cfg::c_scalar_tag_name )
		return std::string( "cfgfile::tag_scalar_t< " ) + value_type +
			std::string( " >" );
	else if( base == cfg::c_no_value_tag_name )
		return std::string( "cfgfile::tag_no_value_t" );
	else if( base == cfg::c_scalar_vector_tag_name )
		return std::string( "cfgfile::tag_scalar_vector_t< " ) + value_type +
				std::string( " >" );
	else if( base == cfg::c_vector_of_tags_tag_name )
		return std::string( "cfgfile::tag_vector_of_tags_t< " ) + value_type +
				std::string( " >" );
	else
		return generate_class_name( base );
} // generate_base_class_name


//
// generate_tag_name_from_class_name
//

static inline std::string generate_tag_name_from_class_name( const std::string & name )
{
	return name;
} // generate_tag_name_from_class_name


//
// bool_to_string
//

static inline std::string bool_to_string( bool value )
{
	if( value )
		return std::string( "true" );
	else
		return std::string( "false" );
} // bool_to_string


//
// generate_fields_in_ctor
//

static inline void generate_fields_in_ctor( std::ostream & stream,
	cfg::const_class_ptr_t c )
{
	foreach( const Cfg::Field & f, c->fields() )
	{
		if( !f.isBase() )
		{
			stream << std::string( "\t\t,\tm_" )
				<< f.name() << std::string( "( *this, std::string( \"" )
				<< f.name() << std::string( "\" ), " )
				<< bool_to_string( f.isRequired() )
				<< std::string( " )\n" );

			if( !f.isConstraintNull() )
			{
				Cfg::constraint_base_t * constr = f.constraint().data();

				switch( constr->type() )
				{
					case Cfg::constraint_base_t::min_max_constraint_type :
					{
						Cfg::min_max_constraint_t * minMax =
							static_cast< Cfg::min_max_constraint_t* > ( constr );

						stream << std::string( "\t\t,\tm_" )
							<< f.name() << std::string( "Constraint( " )
							<< minMax->min() << std::string( ", " )
							<< minMax->max() << std::string( " )\n" );
					}
						break;

					case Cfg::constraint_base_t::one_of_constraint_type :
					{
						stream << std::string( "\t\t,\tm_" )
							<< f.name() << std::string( "Constraint()\n" );
					}
						break;

					default :
						break;
				}
			}
		}
	}
} // generate_fields_in_ctor


//
// generateConstraintsInCtor
//

static inline void generateConstraintsInCtor( std::ostream & stream,
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
						stream << std::string( "\t\tm_" )
							<< f.name() << std::string( ".setConstraint( &m_" )
							<< f.name() << std::string( "Constraint );\n\n" );
					else
						stream << std::string( "\t\tsetConstraint( &m_" )
							<< f.name() << std::string( "Constraint );\n\n" );
				}
					break;

				case Cfg::constraint_base_t::one_of_constraint_type :
				{
					Cfg::one_of_constraint_t * oneOf =
						static_cast< Cfg::one_of_constraint_t* > ( constr );

					foreach( const std::string & s, oneOf->values() )
					{
						stream << std::string( "\t\tm_" )
							<< f.name()
							<< std::string( "Constraint.addValue( " )
							<< s << std::string( " );\n" );
					}

					if( !f.isBase() )
						stream << std::string( "\t\tm_" )
							<< f.name() << std::string( ".setConstraint( &m_" )
							<< f.name() << std::string( "Constraint );\n\n" );
					else
						stream << std::string( "\t\tsetConstraint( &m_" )
							<< f.name() << std::string( "Constraint );\n\n" );
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

static inline std::string generateTypeOfData( const Cfg::Field & f )
{
	switch( f.type() )
	{
		case Cfg::Field::NoValuefield_type_t :
			return std::string( "bool" );

		case Cfg::Field::CustomTagfield_type_t :
		case Cfg::Field::Scalarfield_type_t :
			return f.value_type();

		case Cfg::Field::VectorOfTagsfield_type_t :
		case Cfg::Field::ScalarVectorfield_type_t :
			return std::string( "std::list< " ) + f.value_type() + std::string( " >" );

		default :
			return std::string( "void" );
	}
} // generateTypeOfData


//
// generateDataClass
//

static inline void generateDataClass( std::ostream & stream,
	Cfg::ConstClassPointer c )
{
	stream << std::string( "//\n// " )
		<< c->name() << std::string( "\n//\n\n" );

	stream << std::string( "class " ) << c->name() << std::string( " {\n" );
	stream << std::string( "public:\n" );

	// c_tor and d_tor.
	stream << std::string( "\t" ) << c->name()
		<< std::string( "()\n" );

	int i = 0;

	foreach( const Cfg::Field & f, c->fields() )
	{
		if( !f.defaultValue().isEmpty() )
		{
			if( i == 0 )
				stream << std::string( "\t\t:" );
			else
				stream << std::string( "\t\t," );

			stream << std::string( "\tm_" )
				<< f.name() << std::string( "( " )
				<< f.defaultValue() << std::string( " )\n" );

			++i;
		}
	}

	stream << std::string( "\t{\n"
							 "\t}\n"
							 "\t~" )
		<< c->name() << std::string( "()\n"
									   "\t{\n"
									   "\t}\n\n" );

	// Copy c_tor.
	stream << std::string( "\t" ) << c->name()
		<< std::string( "( const " ) << c->name()
		<< std::string( " & other )\n" );

	i = 0;

	foreach( const Cfg::Field & f, c->fields() )
	{
		if( i == 0 )
			stream << std::string( "\t\t:" );
		else
			stream << std::string( "\t\t," );

		stream << std::string( "\tm_" )
			<< f.name() << std::string( "( other." )
			<< f.name() << std::string( "() )\n" );

		++i;
	}

	stream << std::string( "\t{\n"
							 "\t}\n" );

	// operator =.
	stream << std::string( "\t" ) << c->name()
		<< std::string( " & operator = ( const " ) << c->name()
		<< std::string( " & other )\n"
						  "\t{\n"
						  "\t\tif( this != &other )\n"
						  "\t\t{\n" );

	foreach( const Cfg::Field & f, c->fields() )
	{
		stream << std::string( "\t\t\tm_" )
			<< f.name() << std::string( " = other." )
			<< f.name() << std::string( "();\n" );
	}

	stream << std::string( "\t\t}\n\n"
							 "\t\treturn *this;\n"
							 "\t}\n\n" );

	// Getters and setters.
	foreach( const Cfg::Field & f, c->fields() )
	{
		if( f.type() != Cfg::Field::NoValuefield_type_t )
		{
			stream << std::string( "\tconst " )
				<< generateTypeOfData( f ) << std::string( " & " )
				<< f.name() << std::string( "() const\n"
											  "\t{\n"
											  "\t\treturn m_" )
				<< f.name() << std::string( ";\n"
											  "\t}\n" );

			stream << std::string( "\t" )
				<< generateTypeOfData( f ) << std::string( " & " )
				<< f.name() << std::string( "()\n"
											  "\t{\n"
											  "\t\treturn m_" )
				<< f.name() << std::string( ";\n"
											  "\t}\n" );

			stream << std::string( "\tvoid " )
				<< generate_setter_method_name( f.name() )
				<< std::string( "( const " ) << generateTypeOfData( f )
				<< std::string( " & v )\n"
								  "\t{\n"
								  "\t\tm_" ) << f.name()
				<< std::string( " = v;\n"
								  "\t}\n\n" );
		}
		else
		{
			stream << std::string( "\tbool " )
				<< f.name() << std::string( "() const\n"
											  "\t{\n"
											  "\t\treturn m_" )
				<< f.name() << std::string( ";\n"
											  "\t}\n" );

			stream << std::string( "\tvoid " )
				<< generate_setter_method_name( f.name() )
				<< std::string( "( bool v = true )\n"
								  "\t{\n"
								  "\t\tm_" ) << f.name()
				<< std::string( " = v;\n"
								  "\t}\n\n" );
		}
	}

	// Private members.
	stream << std::string( "private:\n" );

	foreach( const Cfg::Field & f, c->fields() )
	{
		stream << std::string( "\t" ) << generateTypeOfData( f )
			<< std::string( " m_" ) << f.name()
			<< std::string( ";\n" );
	}

	stream << std::string( "}; // class " ) << c->name()
		<< std::string( "\n\n\n" );
} // generateDataClass


//
// generateCfgInit
//

static inline void generateCfgInit( std::ostream & stream,
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
					stream << std::string( "\n\t\tif( m_" )
						<< f.name() << std::string( ".isDefined() )\n"
													  "\t\t\tc." )
						<< generate_setter_method_name( f.name() )
						<< std::string( "( true );\n\n" );
				}
					break;

				case Cfg::Field::Scalarfield_type_t :
				{
					stream << std::string( "\n\t\tif( m_" )
						<< f.name() << std::string( ".isDefined() )\n"
								"\t\t\tc." )
						<< generate_setter_method_name( f.name() )
						<< std::string( "( m_" )
						<< f.name() << std::string( ".value() );\n" );
				}
					break;

				case Cfg::Field::ScalarVectorfield_type_t :
				{
					stream << std::string( "\n\t\tif( m_" )
						<< f.name() << std::string( ".isDefined() )\n"
								"\t\t\tc." )
						<< generate_setter_method_name( f.name() )
						<< std::string( "( m_" )
						<< f.name() << std::string( ".values().toList() );\n" );
				}
					break;

				case Cfg::Field::VectorOfTagsfield_type_t :
				{
					stream << std::string( "\n\t\tif( m_" )
						   << f.name() << std::string( ".isDefined() )\n"
								   "\t\t{\n" )
						<< std::string( "\t\t\tstd::list< " )
						<< f.value_type() << std::string( " > " )
						<< f.name() << std::string( "_" )
						<< generate_tag_name_from_class_name( f.value_type() )
						<< std::string( "List;\n\n" )
						<< std::string( "\t\t\tfor( int i = 0; i < m_" )
						<< f.name() << std::string( ".size(); ++i )\n" )
						<< std::string( "\t\t\t\t" )
						<< f.name() << std::string( "_" )
						<< generate_tag_name_from_class_name( f.value_type() )
						<< std::string( "List.append( m_" ) << f.name()
						<< std::string( ".at( i ).getCfg() );\n\n" )
						<< std::string( "\t\t\tc." )
						<< generate_setter_method_name( f.name() )
						<< std::string( "( " )
						<< f.name() << std::string( "_" )
						<< generate_tag_name_from_class_name( f.value_type() )
						<< std::string( "List );\n" )
						<< std::string( "\t\t}\n" );
				}
					break;

				case Cfg::Field::CustomTagfield_type_t :
				{
					stream << std::string( "\n\t\tif( m_" )
						<< f.name() << std::string( ".isDefined() )\n"
								"\t\t\tc." )
						<< generate_setter_method_name( f.name() )
						<< std::string( "( m_" )
						<< f.name() << std::string( ".getCfg() );\n" );
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
					stream << std::string( "\t\tc." )
						<< generate_setter_method_name( f.name() )
						<< std::string( "( value() );\n" );
				}
					break;

				case Cfg::Field::ScalarVectorfield_type_t :
				{
					stream << std::string( "\t\tc." )
						<< generate_setter_method_name( f.name() )
						<< std::string( "( values().toList() );\n" );
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

static inline void generateCfgSet( std::ostream & stream,
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
					stream << std::string( "\n\t\tif( cfg." )
						<< f.name() << std::string( "() )\n"
													  "\t\t\tm_" )
						<< f.name() << std::string( ".setDefined();\n\n" );
				}
					break;

				case Cfg::Field::Scalarfield_type_t :
				{
					stream << std::string( "\t\tm_" )
						<< f.name()
						<< std::string( ".setValue( cfg." )
						<< f.name() << std::string( "() );\n" );
				}
					break;

				case Cfg::Field::ScalarVectorfield_type_t :
				{
					stream << std::string( "\t\tm_" )
						<< f.name() << std::string( ".setValues( cfg." )
						<< f.name() << std::string( "().toVector() );\n" );
				}
					break;

				case Cfg::Field::VectorOfTagsfield_type_t :
				{
					stream << std::string( "\n\t\tforeach( const " )
						<< f.value_type() << std::string( " & v, cfg." )
						<< f.name() << std::string( "() )\n" )
						<< std::string( "\t\t{\n" )
						<< std::string( "\t\t\tcfgfile::tag_vector_of_tags_t< " )
						<< generate_class_name( f.value_type() )
						<< std::string( " >::PointerToTag p(\n" )
						<< std::string( "\t\t\t\tnew " )
						<< generate_class_name( f.value_type() )
						<< std::string( "( \"" )
						<< f.name() << std::string( "\", " )
						<< bool_to_string( f.isRequired() )
						<< std::string( " ) );\n\n" )
						<< std::string( "\t\t\tp->setCfg( v );\n\n" )
						<< std::string( "\t\t\tm_" ) << f.name()
						<< std::string( ".setValue( p );\n" )
						<< std::string( "\t\t}\n" );
				}
					break;

				case Cfg::Field::CustomTagfield_type_t :
				{
					stream << std::string( "\t\tm_" )
						<< f.name()
						<< std::string( ".setCfg( cfg." )
						<< f.name() << std::string( "() );\n" );
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
					stream << std::string( "\t\tsetValue( cfg." )
						<< f.name() << std::string( "() );\n" );
				}
					break;

				case Cfg::Field::ScalarVectorfield_type_t :
				{
					stream << std::string( "\t\tsetValues( cfg." )
						<< f.name() << std::string( "().toVector() );\n" );
				}
					break;

				default :
					break;
			}
		}
	}

	stream << std::string( "\n\t\tsetDefined();\n" );
} // generateCfgSet


//
// generatePrivateTagMembers
//

static inline void generatePrivateTagMembers( std::ostream & stream,
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
					stream << std::string( "\tcfgfile::tag_no_value_t m_" )
						<< f.name() << std::string( ";\n" );
				}
					break;

				case Cfg::Field::Scalarfield_type_t :
				{
					stream << std::string( "\tcfgfile::tag_scalar_t< " )
						<< f.value_type() << std::string( " > m_" )
						<< f.name() << std::string( ";\n" );
				}
					break;

				case Cfg::Field::ScalarVectorfield_type_t :
				{
					stream << std::string( "\tcfgfile::tag_scalar_tVector< " )
						<< f.value_type() << std::string( " > m_" )
						<< f.name() << std::string( ";\n" );
				}
					break;

				case Cfg::Field::VectorOfTagsfield_type_t :
				{
					stream << std::string( "\tcfgfile::tag_vector_of_tags_t< " )
						<< generate_class_name( f.value_type() )
						<< std::string( " > m_" )
						<< f.name() << std::string( ";\n" );
				}
					break;

				case Cfg::Field::CustomTagfield_type_t :
				{
					stream << std::string( "\t" )
						<< generate_class_name( f.value_type() )
						<< std::string( " m_" )
						<< f.name() << std::string( ";\n" );
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
					stream << std::string( "\tcfgfile::ConstraintMinMax< " )
						<< f.value_type() << std::string( " > m_" )
						<< f.name() << std::string( "Constraint;\n" );
				}
					break;

				case Cfg::constraint_base_t::one_of_constraint_type :
				{
					stream << std::string( "\tcfgfile::constraint_one_of_t< " )
						<< f.value_type() << std::string( " > m_" )
						<< f.name() << std::string( "Constraint;\n" );
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

static inline void generatetag_class_t( std::ostream & stream,
	Cfg::ConstClassPointer c )
{
	const std::string tagClassName = std::string( "Tag" ) + c->name();

	stream << std::string( "//\n"
							 "// " )
		<< tagClassName << std::string( "\n"
									   "//\n\n" );

	stream << std::string( "class " ) << tagClassName
		<< std::string( "\n"
						  "\t:\tpublic " );

	const std::string baseTag = generate_base_class_name( c->baseName(),
		c->baseValueType() );

	stream << baseTag << std::string( "\n" );
	stream << std::string( "{\n" );
	stream << std::string( "public:\n" );

	// c_tors.
	// 1
	const std::string tagName = generate_tag_name_from_class_name( c->name() );

	stream << std::string( "\t" ) << tagClassName
		<< std::string( "()\n"
						  "\t\t:\t" )
		<< baseTag << std::string( "( std::string( \"" )
		<< tagName << std::string( "\" ), true )\n" );

	generate_fields_in_ctor( stream, c );

	stream << std::string( "\t{\n" );

	generateConstraintsInCtor( stream, c );

	stream << std::string( "\t}\n\n" );

	// 2
	stream << std::string( "\texplicit " ) << tagClassName
		<< std::string( "( const " ) << c->name()
		<< std::string( " & cfg )\n"
						  "\t\t:\t" )
		<< baseTag << std::string( "( std::string( \"" )
		<< tagName << std::string( "\" ), true )\n" );

	generate_fields_in_ctor( stream, c );

	stream << std::string( "\t{\n" );

	generateConstraintsInCtor( stream, c );

	stream << std::string( "\t\tsetCfg( cfg );\n" );

	stream << std::string( "\t}\n\n" );

	// 3
	stream << std::string( "\t" ) << tagClassName
		<< std::string( "( const std::string & name, bool is_mandatory )\n" )
		<< std::string( "\t\t:\t" )
		<< baseTag << std::string( "( name, is_mandatory )\n" );

	generate_fields_in_ctor( stream, c );

	stream << std::string( "\t{\n" );

	generateConstraintsInCtor( stream, c );

	stream << std::string( "\t}\n\n" );

	// 4
	stream << std::string( "\t" ) << tagClassName
		<< std::string( "( cfgfile::Tag & owner, const std::string & name, "
						  "bool is_mandatory )\n" )
		<< std::string( "\t\t:\t" )
		<< baseTag << std::string( "( owner, name, is_mandatory )\n" );

	generate_fields_in_ctor( stream, c );

	stream << std::string( "\t{\n" );

	generateConstraintsInCtor( stream, c );

	stream << std::string( "\t}\n\n" );

	// d_tor.
	stream << std::string( "\t~" ) << tagClassName
		<< std::string( "()\n" )
		<< std::string( "\t{\n"
						  "\t}\n\n" );

	// getter.
	stream << std::string( "\t" ) << c->name()
		<< std::string( " getCfg() const\n"
						  "\t{\n"
						  "\t\t" ) << c->name()
		<< std::string( " c;\n" );

	generateCfgInit( stream, c );

	stream << std::string( "\n\t\treturn c;\n"
							 "\t}\n\n" );

	// setter.
	stream << std::string( "\tvoid setCfg( const " )
		<< c->name() << std::string( " & cfg )\n"
									   "\t{\n" );

	generateCfgSet( stream, c );

	stream << std::string( "\t}\n\n" );

	// private members.
	stream << std::string( "private:\n" );

	generatePrivateTagMembers( stream, c );

	stream << std::string( "}; // class " )
		<< tagClassName << std::string( "\n\n" );
} // generatetag_class_t


//
// generateCppClasses
//

static inline void generateCppClasses( std::ostream & stream,
	Cfg::ConstClassPointer c )
{	
	generateDataClass( stream, c );

	generatetag_class_t( stream, c );
} // generateCppClasses

void
cpp_generator_t::generate( std::ostream & stream ) const
{
	unsigned long long index = 0;

	namespace_stack_t nms;

	Cfg::ConstClassPointer c = 0;

	const std::string guard = m_model.includeGuard() + std::string( "__INCLUDED" );

	stream << std::string( "\n#ifndef " ) << guard
		<< std::string( "\n#define " ) << guard
		<< std::string( "\n\n" );

	generate_includes( stream, m_model.global_includes(),
		m_model.relative_includes() );

	while( ( c = m_model.nextClass( index ) ) )
	{
		++index;

		namespace_stack_t tmp;

		const_namespace_ptr_t n = c->parentNamespace();

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
				close_namespace( stream, nms );

			if( nms.isEmpty() )
			{
				nms.swap( tmp );

				start_namespaces( stream, nms );
			}
		}
		else
		{
			while( !nms.isEmpty() )
				close_namespace( stream, nms );
		}

		generateCppClasses( stream, c );
	}

	while( !nms.isEmpty() )
		close_namespace( stream, nms );

	stream << std::string( "#endif // " ) << guard
		<< std::string( "\n" );
}

} /* namespace generator */

} /* namespace cfgfile */
