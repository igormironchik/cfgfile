
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

typedef std::vector< const_namespace_ptr_t > namespace_stack_t;


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
	const std::vector< std::string > & global_includes,
	const std::vector< std::string > & relative_includes )
{
	stream << "// cfgfile include.\n"
		"#include <cfgfile/all.hpp>\n\n";

	stream << "// C++ include.\n"
		<< "#include <vector>\n\n";

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
			std::string( ", Trait >" );
	else if( base == cfg::c_no_value_tag_name )
		return std::string( "cfgfile::tag_no_value_t< Trait >" );
	else if( base == cfg::c_scalar_vector_tag_name )
		return std::string( "cfgfile::tag_scalar_vector_t< " ) + value_type +
				std::string( ", Trait >" );
	else if( base == cfg::c_vector_of_tags_tag_name )
		return std::string( "cfgfile::tag_vector_of_tags_t< " ) + value_type +
				std::string( ", Trait >" );
	else
		return generate_class_name( base );
} // generate_base_class_name


//
// generate_tag_name_from_class_name
//

static inline std::string generate_tag_name_from_class_name(
	const std::string & name, const std::string & tag_name )
{
	if( !tag_name.empty() )
		return tag_name;
	else
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
	for( const cfg::field_t & f : c->fields() )
	{
		if( !f.is_base() )
		{
			stream << std::string( "\t\t,\tm_" )
				<< f.name() << std::string( "( *this, Trait::from_ascii( \"" )
				<< f.name() << std::string( "\" ), " )
				<< bool_to_string( f.is_required() )
				<< std::string( " )\n" );

			if( !f.is_constraint_null() )
			{
				cfg::constraint_base_t * constr = f.constraint().get();

				switch( constr->type() )
				{
					case cfg::constraint_base_t::min_max_constraint_type :
					{
						cfg::min_max_constraint_t * min_max =
							static_cast< cfg::min_max_constraint_t* > ( constr );

						stream << std::string( "\t\t,\tm_" )
							<< f.name() << std::string( "_constraint( " )
							<< min_max->min() << std::string( ", " )
							<< min_max->max() << std::string( " )\n" );
					}
						break;

					case cfg::constraint_base_t::one_of_constraint_type :
					{
						stream << std::string( "\t\t,\tm_" )
							<< f.name() << std::string( "_constraint()\n" );
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
// generate_constraints_in_ctor
//

static inline void generate_constraints_in_ctor( std::ostream & stream,
	cfg::const_class_ptr_t c )
{
	for( const cfg::field_t & f : c->fields() )
	{
		if( !f.is_constraint_null() )
		{
			cfg::constraint_base_t * constr = f.constraint().get();

			switch( constr->type() )
			{
				case cfg::constraint_base_t::min_max_constraint_type :
				{
					if( !f.is_base() )
						stream << std::string( "\t\tm_" )
							<< f.name() << std::string( ".set_constraint( &m_" )
							<< f.name() << std::string( "_constraint );\n\n" );
					else
						stream << std::string( "\t\tthis->set_constraint( &m_" )
							<< f.name() << std::string( "_constraint );\n\n" );
				}
					break;

				case cfg::constraint_base_t::one_of_constraint_type :
				{
					cfg::one_of_constraint_t * one_of =
						static_cast< cfg::one_of_constraint_t* > ( constr );

					for( const std::string & s : one_of->values() )
					{
						stream << std::string( "\t\tm_" )
							<< f.name()
							<< std::string( "_constraint.add_value( " )
							<< s << std::string( " );\n" );
					}

					if( !f.is_base() )
						stream << std::string( "\t\tm_" )
							<< f.name() << std::string( ".set_constraint( &m_" )
							<< f.name() << std::string( "_constraint );\n\n" );
					else
						stream << std::string( "\t\tthis->set_constraint( &m_" )
							<< f.name() << std::string( "_constraint );\n\n" );
				}
					break;

				default :
					break;
			}
		}
	}
} // generate_constraints_in_ctor


//
// generate_type_of_data
//

static inline std::string generate_type_of_data( const cfg::field_t & f )
{
	switch( f.type() )
	{
		case cfg::field_t::no_value_field_type :
			return std::string( "bool" );

		case cfg::field_t::custom_tag_field_type :
		case cfg::field_t::scalar_field_type :
			return f.value_type();

		case cfg::field_t::vector_of_tags_field_type :
		case cfg::field_t::scalar_vector_field_type :
			return std::string( "std::vector< " ) + f.value_type() + std::string( " >" );

		default :
			return std::string( "void" );
	}
} // generate_type_of_data


//
// generate_data_class
//

static inline void generate_data_class( std::ostream & stream,
	cfg::const_class_ptr_t c )
{
	stream << std::string( "//\n// " )
		<< c->name() << std::string( "\n//\n\n" );

	stream << std::string( "class " ) << c->name() << std::string( " {\n" );
	stream << std::string( "public:\n" );

	// c_tor and d_tor.
	stream << std::string( "\t" ) << c->name()
		<< std::string( "()\n" );

	int i = 0;

	for( const cfg::field_t & f : c->fields() )
	{
		if( !f.default_value().empty() )
		{
			if( i == 0 )
				stream << std::string( "\t\t:" );
			else
				stream << std::string( "\t\t," );

			stream << std::string( "\tm_" )
				<< f.name() << std::string( "( " )
				<< f.default_value() << std::string( " )\n" );

			++i;
		}
	}

	stream << std::string( "\t{\n"
							 "\t}\n"
							 "\t~" )
		<< c->name() << std::string( "()\n"
									   "\t{\n"
									   "\t}\n\n" );

	// Getters and setters.
	for( const cfg::field_t & f : c->fields() )
	{
		if( f.type() != cfg::field_t::no_value_field_type )
		{
			stream << std::string( "\tconst " )
				<< generate_type_of_data( f ) << std::string( " & " )
				<< f.name() << std::string( "() const\n"
											  "\t{\n"
											  "\t\treturn m_" )
				<< f.name() << std::string( ";\n"
											  "\t}\n" );

			stream << std::string( "\t" )
				<< generate_type_of_data( f ) << std::string( " & " )
				<< f.name() << std::string( "()\n"
											  "\t{\n"
											  "\t\treturn m_" )
				<< f.name() << std::string( ";\n"
											  "\t}\n" );

			stream << std::string( "\tvoid " )
				<< generate_setter_method_name( f.name() )
				<< std::string( "( const " ) << generate_type_of_data( f )
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

	for( const cfg::field_t & f : c->fields() )
	{
		stream << std::string( "\t" ) << generate_type_of_data( f )
			<< std::string( " m_" ) << f.name()
			<< std::string( ";\n" );
	}

	stream << std::string( "}; // class " ) << c->name()
		<< std::string( "\n\n\n" );
} // generate_data_class


//
// generate_cfg_init
//

static inline void generate_cfg_init( std::ostream & stream,
	cfg::const_class_ptr_t c )
{
	for( const cfg::field_t & f : c->fields() )
	{
		if( !f.is_base() )
		{
			switch( f.type() )
			{
				case cfg::field_t::no_value_field_type :
				{
					stream << std::string( "\n\t\tif( m_" )
						<< f.name() << std::string( ".is_defined() )\n"
													  "\t\t\tc." )
						<< generate_setter_method_name( f.name() )
						<< std::string( "( true );\n\n" );
				}
					break;

				case cfg::field_t::scalar_field_type :
				{
					stream << std::string( "\n\t\tif( m_" )
						<< f.name() << std::string( ".is_defined() )\n"
								"\t\t\tc." )
						<< generate_setter_method_name( f.name() )
						<< std::string( "( m_" )
						<< f.name() << std::string( ".value() );\n" );
				}
					break;

				case cfg::field_t::scalar_vector_field_type :
				{
					stream << std::string( "\n\t\tif( m_" )
						<< f.name() << std::string( ".is_defined() )\n"
								"\t\t\tc." )
						<< generate_setter_method_name( f.name() )
						<< std::string( "( m_" )
						<< f.name() << std::string( ".values() );\n" );
				}
					break;

				case cfg::field_t::vector_of_tags_field_type :
				{
					stream << std::string( "\n\t\tif( m_" )
						   << f.name() << std::string( ".is_defined() )\n"
								   "\t\t{\n" )
						<< std::string( "\t\t\tstd::vector< " )
						<< f.value_type() << std::string( " > " )
						<< f.name() << std::string( "_" )
						<< std::string( "_;\n\n" )
						<< std::string( "\t\t\tfor( std::size_t i = 0; i < m_" )
						<< f.name() << std::string( ".size(); ++i )\n" )
						<< std::string( "\t\t\t\t" )
						<< f.name() << std::string( "_" )
						<< std::string( "_.push_back( m_" ) << f.name()
						<< std::string( ".at( i ).get_cfg() );\n\n" )
						<< std::string( "\t\t\tc." )
						<< generate_setter_method_name( f.name() )
						<< std::string( "( " )
						<< f.name() << std::string( "_" )
						<< std::string( "_ );\n" )
						<< std::string( "\t\t}\n" );
				}
					break;

				case cfg::field_t::custom_tag_field_type :
				{
					stream << std::string( "\n\t\tif( m_" )
						<< f.name() << std::string( ".is_defined() )\n"
								"\t\t\tc." )
						<< generate_setter_method_name( f.name() )
						<< std::string( "( m_" )
						<< f.name() << std::string( ".get_cfg() );\n" );
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
				case cfg::field_t::scalar_field_type :
				{
					stream << std::string( "\t\tc." )
						<< generate_setter_method_name( f.name() )
						<< std::string( "( this->value() );\n" );
				}
					break;

				case cfg::field_t::scalar_vector_field_type :
				{
					stream << std::string( "\t\tc." )
						<< generate_setter_method_name( f.name() )
						<< std::string( "( this->values() );\n" );
				}
					break;

				default :
					break;
			}
		}
	}
} // generate_cfg_init


//
// generate_cfg_set
//

static inline void generate_cfg_set( std::ostream & stream,
	cfg::const_class_ptr_t c )
{
	for( const cfg::field_t & f : c->fields() )
	{
		if( !f.is_base() )
		{
			switch( f.type() )
			{
				case cfg::field_t::no_value_field_type :
				{
					stream << std::string( "\n\t\tif( cfg." )
						<< f.name() << std::string( "() )\n"
													  "\t\t\tm_" )
						<< f.name() << std::string( ".set_defined();\n\n" );
				}
					break;

				case cfg::field_t::scalar_field_type :
				{
					stream << std::string( "\t\tm_" )
						<< f.name()
						<< std::string( ".set_value( cfg." )
						<< f.name() << std::string( "() );\n" );
				}
					break;

				case cfg::field_t::scalar_vector_field_type :
				{
					stream << std::string( "\t\tm_" )
						<< f.name() << std::string( ".set_values( cfg." )
						<< f.name() << std::string( "() );\n" );
				}
					break;

				case cfg::field_t::vector_of_tags_field_type :
				{
					stream << std::string( "\n\t\tfor( const " )
						<< f.value_type() << std::string( " & v : cfg." )
						<< f.name() << std::string( "() )\n" )
						<< std::string( "\t\t{\n" )
						<< std::string( "\t\t\ttypename cfgfile::tag_vector_of_tags_t< " )
						<< generate_class_name( f.value_type() )
						<< std::string( "< Trait >, Trait >::ptr_to_tag_t p(\n" )
						<< std::string( "\t\t\t\tnew " )
						<< generate_class_name( f.value_type() )
						<< std::string( "< Trait >( \"" )
						<< f.name() << std::string( "\", " )
						<< bool_to_string( f.is_required() )
						<< std::string( " ) );\n\n" )
						<< std::string( "\t\t\tp->set_cfg( v );\n\n" )
						<< std::string( "\t\t\tm_" ) << f.name()
						<< std::string( ".set_value( p );\n" )
						<< std::string( "\t\t}\n" );
				}
					break;

				case cfg::field_t::custom_tag_field_type :
				{
					stream << std::string( "\t\tm_" )
						<< f.name()
						<< std::string( ".set_cfg( cfg." )
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
				case cfg::field_t::scalar_field_type :
				{
					stream << std::string( "\t\tthis->set_value( cfg." )
						<< f.name() << std::string( "() );\n" );
				}
					break;

				case cfg::field_t::scalar_vector_field_type :
				{
					stream << std::string( "\t\tthis->set_values( cfg." )
						<< f.name() << std::string( "() );\n" );
				}
					break;

				default :
					break;
			}
		}
	}

	if( c->fields().empty() )
		stream << std::string( "\t\t(void) cfg;" );

	stream << std::string( "\n\t\tthis->set_defined();\n" );
} // generate_cfg_set


//
// generate_private_tag_members
//

static inline void generate_private_tag_members( std::ostream & stream,
	cfg::const_class_ptr_t c )
{
	for( const cfg::field_t & f : c->fields() )
	{
		if( !f.is_base() )
		{
			switch( f.type() )
			{
				case cfg::field_t::no_value_field_type :
				{
					stream << std::string( "\tcfgfile::tag_no_value_t< Trait > m_" )
						<< f.name() << std::string( ";\n" );
				}
					break;

				case cfg::field_t::scalar_field_type :
				{
					stream << std::string( "\tcfgfile::tag_scalar_t< " )
						<< f.value_type() << std::string( ", Trait > m_" )
						<< f.name() << std::string( ";\n" );
				}
					break;

				case cfg::field_t::scalar_vector_field_type :
				{
					stream << std::string( "\tcfgfile::tag_scalar_vector_t< " )
						<< f.value_type() << std::string( ", Trait > m_" )
						<< f.name() << std::string( ";\n" );
				}
					break;

				case cfg::field_t::vector_of_tags_field_type :
				{
					stream << std::string( "\tcfgfile::tag_vector_of_tags_t< " )
						<< generate_class_name( f.value_type() )
						<< "< Trait >"
						<< std::string( ", Trait > m_" )
						<< f.name() << std::string( ";\n" );
				}
					break;

				case cfg::field_t::custom_tag_field_type :
				{
					stream << std::string( "\t" )
						<< generate_class_name( f.value_type() )
						<< std::string( "< Trait > m_" )
						<< f.name() << std::string( ";\n" );
				}
					break;

				default :
					break;
			}
		}

		if( !f.is_constraint_null() )
		{
			cfg::constraint_base_t * constr = f.constraint().get();

			switch( constr->type() )
			{
				case cfg::constraint_base_t::min_max_constraint_type :
				{
					stream << std::string( "\tcfgfile::constraint_min_max_t< " )
						<< f.value_type() << std::string( " > m_" )
						<< f.name() << std::string( "_constraint;\n" );
				}
					break;

				case cfg::constraint_base_t::one_of_constraint_type :
				{
					stream << std::string( "\tcfgfile::constraint_one_of_t< " )
						<< f.value_type() << std::string( " > m_" )
						<< f.name() << std::string( "_constraint;\n" );
				}
					break;

				default :
					break;
			}
		}
	}
} // generate_private_tag_members


//
// generatetag_class_t
//

static inline void generate_tag_class( std::ostream & stream,
	cfg::const_class_ptr_t c )
{
	const std::string tag_class_name = std::string( "tag_" ) + c->name();

	stream << std::string( "//\n"
							 "// " )
		<< tag_class_name << std::string( "\n"
									   "//\n\n" );

	stream << std::string( "template< typename Trait >\n" );
	stream << std::string( "class " ) << tag_class_name
		<< std::string( "\n"
						  "\t:\tpublic " );

	const std::string base_tag = generate_base_class_name( c->base_name(),
		c->base_value_type() );

	stream << base_tag << std::string( "\n" );
	stream << std::string( "{\n" );
	stream << std::string( "public:\n" );

	// c_tors.
	// 1
	const std::string tag_name = generate_tag_name_from_class_name( c->name(),
		c->tag_name() );

	stream << std::string( "\t" ) << tag_class_name
		<< std::string( "()\n"
						  "\t\t:\t" )
		<< base_tag << std::string( "( Trait::from_ascii( \"" )
		<< tag_name << std::string( "\" ), true )\n" );

	generate_fields_in_ctor( stream, c );

	stream << std::string( "\t{\n" );

	generate_constraints_in_ctor( stream, c );

	stream << std::string( "\t}\n\n" );

	// 2
	stream << std::string( "\texplicit " ) << tag_class_name
		<< std::string( "( const " ) << c->name()
		<< std::string( " & cfg )\n"
						  "\t\t:\t" )
		<< base_tag << std::string( "( Trait::from_ascii( \"" )
		<< tag_name << std::string( "\" ), true )\n" );

	generate_fields_in_ctor( stream, c );

	stream << std::string( "\t{\n" );

	generate_constraints_in_ctor( stream, c );

	stream << std::string( "\t\tset_cfg( cfg );\n" );

	stream << std::string( "\t}\n\n" );

	// 3
	stream << std::string( "\t" ) << tag_class_name
		<< std::string( "( const typename Trait::string_t & name, bool is_mandatory )\n" )
		<< std::string( "\t\t:\t" )
		<< base_tag << std::string( "( name, is_mandatory )\n" );

	generate_fields_in_ctor( stream, c );

	stream << std::string( "\t{\n" );

	generate_constraints_in_ctor( stream, c );

	stream << std::string( "\t}\n\n" );

	// 4
	stream << std::string( "\t" ) << tag_class_name
		<< std::string( "( cfgfile::tag_t< Trait > & owner, const typename Trait::string_t & name, "
						  "bool is_mandatory )\n" )
		<< std::string( "\t\t:\t" )
		<< base_tag << std::string( "( owner, name, is_mandatory )\n" );

	generate_fields_in_ctor( stream, c );

	stream << std::string( "\t{\n" );

	generate_constraints_in_ctor( stream, c );

	stream << std::string( "\t}\n\n" );

	// d_tor.
	stream << std::string( "\t~" ) << tag_class_name
		<< std::string( "()\n" )
		<< std::string( "\t{\n"
						  "\t}\n\n" );

	// getter.
	stream << std::string( "\t" ) << c->name()
		<< std::string( " get_cfg() const\n"
						  "\t{\n"
						  "\t\t" ) << c->name()
		<< std::string( " c;\n" );

	generate_cfg_init( stream, c );

	stream << std::string( "\n\t\treturn c;\n"
							 "\t}\n\n" );

	// setter.
	stream << std::string( "\tvoid set_cfg( const " )
		<< c->name() << std::string( " & cfg )\n"
									   "\t{\n" );

	generate_cfg_set( stream, c );

	stream << std::string( "\t}\n\n" );

	// private members.
	stream << std::string( "private:\n" );

	generate_private_tag_members( stream, c );

	stream << std::string( "}; // class " )
		<< tag_class_name << std::string( "\n\n" );
} // generatetag_class_t


//
// generate_cpp_classes
//

static inline void generate_cpp_classes( std::ostream & stream,
	cfg::const_class_ptr_t c )
{	
	generate_data_class( stream, c );

	generate_tag_class( stream, c );
} // generate_cpp_classes

void
cpp_generator_t::generate( std::ostream & stream ) const
{
	unsigned long long index = 0;

	namespace_stack_t nms;

	cfg::const_class_ptr_t c = 0;

	const std::string guard = m_model.include_guard() +
		std::string( "__INCLUDED" );

	stream << std::string( "\n#ifndef " ) << guard
		<< std::string( "\n#define " ) << guard
		<< std::string( "\n\n" );

	generate_includes( stream, m_model.global_includes(),
		m_model.relative_includes() );

	while( ( c = m_model.next_class( index ) ) )
	{
		++index;

		namespace_stack_t tmp;

		const_namespace_ptr_t n = c->parent_namespace();

		while( n )
		{
			if( !n->name().empty() )
				tmp.push_back( n );

			n = n->parent_namespace();
		}

		if( !tmp.empty() )
		{
			std::reverse( tmp.begin(), tmp.end() );

			while( !nms.empty() && nms.back() != tmp.back() )
				close_namespace( stream, nms );

			if( nms.empty() )
			{
				nms.swap( tmp );

				start_namespaces( stream, nms );
			}
		}
		else
		{
			while( !nms.empty() )
				close_namespace( stream, nms );
		}

		generate_cpp_classes( stream, c );
	}

	while( !nms.empty() )
		close_namespace( stream, nms );

	stream << std::string( "#endif // " ) << guard
		<< std::string( "\n" );
}

} /* namespace generator */

} /* namespace cfgfile */
