
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

// generator_t cfg include.
#include "cfg.hpp"

// C++ include.
#include <algorithm>


namespace cfgfile {

namespace generator {

namespace cfg {

//
// constraint_base_t
//

constraint_base_t::constraint_base_t()
{
}

constraint_base_t::~constraint_base_t()
{
}


//
// min_max_constraint_t
//

min_max_constraint_t::min_max_constraint_t( const std::string & min_value,
	const std::string & max_value )
	:	m_min( min_value )
	,	m_max( max_value )
{
}

min_max_constraint_t::~min_max_constraint_t()
{
}

constraint_base_t::constraint_type_t
min_max_constraint_t::type() const
{
	return min_max_constraint_type;
}

const std::string &
min_max_constraint_t::min() const
{
	return m_min;
}

const std::string &
min_max_constraint_t::max() const
{
	return m_max;
}


//
// one_of_constraint_t
//

one_of_constraint_t::one_of_constraint_t( const std::vector< std::string > & values )
	:	m_values( values )
{
}

one_of_constraint_t::~one_of_constraint_t()
{

}

constraint_base_t::constraint_type_t
one_of_constraint_t::type() const
{
	return one_of_constraint_type;
}

const std::vector< std::string > &
one_of_constraint_t::values() const
{
	return m_values;
}


//
// field_t
//

field_t::field_t()
	:	m_type( unknown_field_type )
	,	m_line_number( -1 )
	,	m_column_number( -1 )
	,	m_is_required( false )
	,	m_is_base( false )
{
}

field_t::~field_t()
{
}

bool
field_t::operator == ( const std::string & n ) const
{
	return ( name() == n );
}

field_t::field_type_t
field_t::type() const
{
	return m_type;
}

void
field_t::set_type( field_type_t t )
{
	m_type = t;
}

const std::string &
field_t::value_type() const
{
	return m_value_type;
}

void
field_t::set_value_type( const std::string & t )
{
	m_value_type = t;
}

const std::string &
field_t::name() const
{
	return m_name;
}

void
field_t::set_name( const std::string & n )
{
	m_name = n;
}

bool
field_t::is_constraint_null() const
{
	return ( m_constraint.get() == nullptr );
}

const std::shared_ptr< constraint_base_t > &
field_t::constraint() const
{
	return m_constraint;
}

void
field_t::set_constraint( const std::shared_ptr< constraint_base_t > & c )
{
	m_constraint = c;
}

long long
field_t::line_number() const
{
	return m_line_number;
}

void
field_t::set_line_number( long long num )
{
	m_line_number = num;
}

long long
field_t::column_number() const
{
	return m_column_number;
}

void
field_t::set_column_number( long long num )
{
	m_column_number = num;
}

bool
field_t::is_required() const
{
	return m_is_required;
}

void
field_t::set_required( bool on )
{
	m_is_required = on;
}

const std::string &
field_t::default_value() const
{
	return m_default_value;
}

void
field_t::set_default_value( const std::string & value )
{
	m_default_value = value;
}

bool
field_t::is_base() const
{
	return m_is_base;
}

void
field_t::set_base( bool on )
{
	m_is_base = on;
}


//
// class_t
//

class_t::class_t()
	:	m_base_name( c_no_value_tag_name )
	,	m_line_number( -1 )
	,	m_column_number( -1 )
	,	m_index( 0 )
	,	m_parent( 0 )
{
}

class_t::~class_t()
{
}

class_t::class_t( const class_t & other )
	:	m_name( other.name() )
	,	m_tag_name( other.tag_name() )
	,	m_base_name( other.base_name() )
	,	m_base_value_type( other.base_value_type() )
	,	m_fields( other.fields() )
	,	m_line_number( other.line_number() )
	,	m_column_number( other.column_number() )
	,	m_index( other.index() )
	,	m_parent( nullptr )
{
}

class_t &
class_t::operator = ( const class_t & other )
{
	if( this != &other )
	{
		m_name = other.name();
		m_tag_name = other.tag_name();
		m_base_name = other.base_name();
		m_base_value_type = other.base_value_type();
		m_fields = other.fields();
		m_line_number = other.line_number();
		m_column_number = other.column_number();
		m_index = other.index();
		m_parent = nullptr;
	}

	return *this;
}

bool
class_t::operator == ( const std::string & n ) const
{
	return ( name() == n );
}

const std::string &
class_t::name() const
{
	return m_name;
}

const std::string &
class_t::tag_name() const
{
	return m_tag_name;
}

void
class_t::set_tag_name( const std::string & n )
{
	m_tag_name = n;
}

void
class_t::set_name( const std::string & n )
{
	m_name = n;
}

const std::string &
class_t::base_name() const
{
	return m_base_name;
}

void
class_t::set_base_name( const std::string & n )
{
	m_base_name = n;
}

const std::string &
class_t::base_value_type() const
{
	return m_base_value_type;
}

void
class_t::set_base_value_type( const std::string & t )
{
	m_base_value_type = t;
}

const std::vector< field_t > &
class_t::fields() const
{
	return m_fields;
}

const field_t *
class_t::field_by_name( const std::string & n ) const
{
	if( n.empty() )
		return 0;

	std::vector< field_t >::const_iterator it = m_fields.cbegin();
	std::vector< field_t >::const_iterator last = m_fields.cend();

	for( ; it != last; ++it )
	{
		if( it->name() == n )
			return &(*it);
	}

	return 0;
}

field_t *
class_t::field_by_name( const std::string & n )
{
	if( n.empty() )
		return 0;

	std::vector< field_t >::iterator it = m_fields.begin();
	std::vector< field_t >::iterator last = m_fields.end();

	for( ; it != last; ++it )
	{
		if( it->name() == n )
			return &(*it);
	}

	return 0;
}

void
class_t::add_field( const field_t & f )
{
	m_fields.push_back( f );
}

long long
class_t::line_number() const
{
	return m_line_number;
}

void
class_t::set_line_number( long long num )
{
	m_line_number = num;
}

long long
class_t::column_number() const
{
	return m_column_number;
}

void
class_t::set_column_number( long long num )
{
	m_column_number = num;
}

unsigned long long
class_t::index() const
{
	return m_index;
}

void
class_t::set_index( unsigned long long i ) const
{
	m_index=  i;
}

const namespace_t *
class_t::parent_namespace() const
{
	return m_parent;
}

void
class_t::set_parent_namespace( const namespace_t * p ) const
{
	m_parent = p;
}


//
// namespace_t
//

namespace_t::namespace_t()
	:	m_line_number( -1 )
	,	m_column_number( -1 )
	,	m_parent( 0 )
{
}

namespace_t::~namespace_t()
{
}

namespace_t::namespace_t( const namespace_t & other )
	:	m_name( other.name() )
	,	m_nested_namespaces( other.all_nested() )
	,	m_classes( other.classes() )
	,	m_line_number( other.line_number() )
	,	m_column_number( other.column_number() )
	,	m_parent( nullptr )
{
}

namespace_t &
namespace_t::operator = ( const namespace_t & other )
{
	if( this != &other )
	{
		m_name = other.name();
		m_nested_namespaces = other.all_nested();
		m_classes = other.classes();
		m_line_number = other.line_number();
		m_column_number = other.column_number();
		m_parent = nullptr;
	}

	return *this;
}

const std::string &
namespace_t::name() const
{
	return m_name;
}

void
namespace_t::set_name( const std::string & n )
{
	m_name = n;
}

const std::vector< namespace_t > &
namespace_t::all_nested() const
{
	return m_nested_namespaces;
}

std::vector< const namespace_t* >
namespace_t::nested( const std::string & n ) const
{
	std::vector< const namespace_t* > res;

	if( n.empty() )
		return res;

	std::vector< namespace_t >::const_iterator it = m_nested_namespaces.cbegin();
	std::vector< namespace_t >::const_iterator last = m_nested_namespaces.cend();

	for( ; it != last; ++it )
	{
		if( it->name() == n )
			res.push_back( &(*it) );
	}

	return res;
}

std::vector< namespace_t* >
namespace_t::nested( const std::string & n )
{
	std::vector< namespace_t* > res;

	if( n.empty() )
		return res;

	std::vector< namespace_t >::iterator it = m_nested_namespaces.begin();
	std::vector< namespace_t >::iterator last = m_nested_namespaces.end();

	for( ; it != last; ++it )
	{
		if( it->name() == n )
			res.push_back( &(*it) );
	}

	return res;
}

void
namespace_t::add_namespace( const namespace_t & n )
{
	m_nested_namespaces.push_back( n );
}

const std::vector< class_t > &
namespace_t::classes() const
{
	return m_classes;
}

const class_t *
namespace_t::class_by_name( const std::string & n ) const
{
	if( n.empty() )
		return 0;

	std::vector< class_t >::const_iterator it = m_classes.cbegin();
	std::vector< class_t >::const_iterator last = m_classes.cend();

	for( ; it != last; ++it )
	{
		if( it->name() == n )
			return &(*it);
	}

	return 0;
}

class_t *
namespace_t::class_by_name( const std::string & n )
{
	if( n.empty() )
		return 0;

	std::vector< class_t >::iterator it = m_classes.begin();
	std::vector< class_t >::iterator last = m_classes.end();

	for( ; it != last; ++it )
	{
		if( it->name() == n )
			return &(*it);
	}

	return 0;
}

void
namespace_t::add_class( const class_t & c )
{
	m_classes.push_back( c );
}

long long
namespace_t::line_number() const
{
	return m_line_number;
}

void
namespace_t::set_line_number( long long num )
{
	m_line_number = num;
}

long long
namespace_t::column_number() const
{
	return m_column_number;
}

void
namespace_t::set_column_number( long long num )
{
	m_column_number = num;
}

const namespace_t *
namespace_t::parent_namespace() const
{
	return m_parent;
}

void
namespace_t::set_parent_namespace( const namespace_t * p ) const
{
	m_parent = p;
}


//
// model_t
//

model_t::model_t()
{
}

model_t::~model_t()
{
}

model_t::model_t( const model_t & other )
	:	m_root( other.root_namespace() )
	,	m_global_includes( other.global_includes() )
	,	m_relative_includes( other.relative_includes() )
	,	m_include_guard( other.include_guard() )
{
}

model_t &
model_t::operator = ( const model_t & other )
{
	if( this != &other )
	{
		m_root = other.root_namespace();
		m_global_includes = other.global_includes();
		m_relative_includes = other.relative_includes();
		m_include_guard = other.include_guard();
	}

	return *this;
}

const namespace_t &
model_t::root_namespace() const
{
	return m_root;
}

namespace_t &
model_t::root_namespace()
{
	return m_root;
}

void
model_t::set_root_namespace( const namespace_t & n )
{
	m_root = n;
}

const std::vector< std::string > &
model_t::global_includes() const
{
	return m_global_includes;
}

void
model_t::set_global_includes( const std::vector< std::string > & inc )
{
	m_global_includes = inc;
}

void
model_t::add_global_include( const std::string & inc )
{
	m_global_includes.push_back( inc );
}

const std::vector< std::string > &
model_t::relative_includes() const
{
	return m_relative_includes;
}

void
model_t::set_relative_includes( const std::vector< std::string > & inc )
{
	m_relative_includes = inc;
}

void
model_t::add_relative_include( const std::string & inc )
{
	m_relative_includes.push_back( inc );
}

void extract_and_bind_all_classes( const namespace_t & root,
	std::vector< const_class_ptr_t > & data )
{
	{
		std::vector< class_t >::const_iterator it = root.classes().cbegin();
		std::vector< class_t >::const_iterator last = root.classes().cend();

		for( ; it != last; ++it )
		{
			data.push_back( &(*it) );
			it->set_parent_namespace( &root );
		}
	}

	{
		std::vector< namespace_t >::const_iterator it = root.all_nested().cbegin();
		std::vector< namespace_t >::const_iterator last = root.all_nested().cend();

		for( ; it != last; ++it )
		{
			it->set_parent_namespace( &root );
			extract_and_bind_all_classes( *it, data );
		}

	}
}

struct const_class_ptr_less {
	bool operator () ( const const_class_ptr_t & c1,
		const const_class_ptr_t & c2 )
	{
		if( c1->line_number() < c2->line_number() )
			return true;
		else if( c1->line_number() == c2->line_number() )
			return ( c1->column_number() < c2->column_number() );
		else
			return false;
	}
};

void
model_t::prepare()
{
	m_indexes.clear();

	std::vector< const_class_ptr_t > sorted;

	extract_and_bind_all_classes( m_root, sorted );

	std::sort( sorted.begin(), sorted.end(), const_class_ptr_less() );

	unsigned long long index = 1;

	for( const const_class_ptr_t & c : sorted )
	{
		c->set_index( index );

		m_indexes.insert( std::make_pair( index, c ) );

		++index;
	}
}

void
model_t::check() const
{
	std::vector< std::string > classes;

	const_class_ptr_t c = 0;

	unsigned long long index = 0;

	const bool included = is_included();

	while( ( c = next_class( index ) ) )
	{
		check_class( *c, classes, included );
		++index;
	}
}

const_class_ptr_t
model_t::next_class( unsigned long long index ) const
{
	index += 1;

	if( m_indexes.find( index ) != m_indexes.cend() )
		return m_indexes.at( index );
	else
		return 0;
}

static inline std::string full_name( const class_t & c,
	const std::string & start_name = std::string() )
{
	std::string name = ( start_name.empty() ? c.name() : start_name );

	const namespace_t * nm = c.parent_namespace();

	while( nm )
	{
		if( !nm->name().empty() )
		{
			name.insert( 0, c_namespace_separator );
			name.insert( 0, nm->name() );
		}

		nm = nm->parent_namespace();
	}

	return name;
}

static inline bool
check_is_class_defined( const std::string & class_to_check,
	const std::vector< std::string > & prev_defined_classes )
{
	return ( std::find( prev_defined_classes.cbegin(),
		prev_defined_classes.cend(), class_to_check ) !=
			prev_defined_classes.cend() );
}

void
model_t::check_class( const class_t & c,
	std::vector< std::string > & prev_defined_classes, bool included ) const
{
	const std::string class_name = full_name( c );

	if( std::find( prev_defined_classes.cbegin(), prev_defined_classes.cend(),
		class_name ) != prev_defined_classes.cend() )
			throw cfgfile::exception_t< cfgfile::string_trait_t >( std::string( "Redefinition of class \"" ) +
				class_name + "\". Line " + std::to_string( c.line_number() ) +
				", column " + std::to_string( c.column_number() ) +	"." );

	prev_defined_classes.push_back( class_name );

	std::vector< std::string > fields;

	for( const field_t & f : c.fields() )
	{
		if( f.is_base() )
		{
			switch( f.type() )
			{
				case field_t::scalar_field_type :
				case field_t::scalar_vector_field_type :
				{
					if( f.name().empty() )
						throw cfgfile::exception_t< cfgfile::string_trait_t >( std::string(
							"Empty name of base type. Line " ) +
							std::to_string( f.line_number() ) +
							", column " + std::to_string( f.column_number() ) +
							"." );
				}
					break;

				default :
					break;
			}
		}
		else if( f.name().empty() )
			throw cfgfile::exception_t< cfgfile::string_trait_t >( std::string(
				"Empty name of field of class \"" ) + class_name +
				"\". Line " + std::to_string( f.line_number() ) +
				", column " + std::to_string( f.column_number() ) +
				"." );

		if( std::find( fields.cbegin(), fields.cend(), f.name() ) !=
			fields.cend() )
				throw cfgfile::exception_t< cfgfile::string_trait_t >( std::string( "Field \"" ) +
					f.name() + "\" already defined in class \"" +
					class_name + "\". Line " +
					std::to_string( f.line_number() ) +
					", column " + std::to_string( f.column_number()  ) + "." );
		else
			fields.push_back( f.name() );

		if( !included )
		{
			if( f.type() == field_t::custom_tag_field_type ||
				f.type() == field_t::vector_of_tags_field_type )
			{
				if( !check_is_class_defined( f.value_type(),
					prev_defined_classes ) )
				{
					throw cfgfile::exception_t< cfgfile::string_trait_t >( std::string( "Value type \"" ) +
						f.value_type() + "\" of member \"" +
						f.name() + "\" of class \"" +
						class_name + "\" wasn't defined. Line " +
						std::to_string( c.line_number() ) +
						", column " + std::to_string( c.column_number() ) + "." );
				}
			}
		}
	}
}

bool
model_t::is_included() const
{
	return ( !m_global_includes.empty() || !m_relative_includes.empty() );
}

const std::string &
model_t::include_guard() const
{
	return m_include_guard;
}

void
model_t::set_include_guard( const std::string & guard )
{
	m_include_guard = guard;
}


//
// tag_min_max_constraint_t
//

tag_min_max_constraint_t::tag_min_max_constraint_t(
	cfgfile::tag_t< cfgfile::string_trait_t > & owner )
	:	cfgfile::tag_no_value_t<> ( owner, c_min_max_constraint_tag_name, false )
	,	m_min( *this, c_min_tag_name, true )
	,	m_max( *this, c_max_tag_name, true )
{
}

tag_min_max_constraint_t::~tag_min_max_constraint_t()
{
}

std::shared_ptr< min_max_constraint_t >
tag_min_max_constraint_t::cfg() const
{
	return std::shared_ptr< min_max_constraint_t > ( new min_max_constraint_t(
		m_min.value(), m_max.value() ) );
}


//
// tag_one_of_constraint_t
//

tag_one_of_constraint_t::tag_one_of_constraint_t(
	cfgfile::tag_t< cfgfile::string_trait_t > & owner )
	:	cfgfile::tag_scalar_vector_t< std::string > ( owner,
			c_one_of_constraint_tag_name, false )
{
}

tag_one_of_constraint_t::~tag_one_of_constraint_t()
{
}

std::shared_ptr< one_of_constraint_t >
tag_one_of_constraint_t::cfg() const
{
	return std::shared_ptr< one_of_constraint_t > (
		new one_of_constraint_t( values() ) );
}


//
// tag_field_t
//

tag_field_t::tag_field_t( const std::string & name, bool is_mandatory )
	:	cfgfile::tag_no_value_t<> ( name, is_mandatory )
	,	m_name( *this, c_field_name_tag_name, true )
	,	m_value_type( *this, c_value_type_tag_name, false )
	,	m_min_max_constraint( *this )
	,	m_one_of_constraint( *this )
	,	m_is_required( *this, c_required_tag_name, false )
	,	m_default_value( *this, c_default_value_tag_name, false )
{
}

tag_field_t::~tag_field_t()
{
}

static inline field_t::field_type_t field_type_from_string( const std::string & type )
{
	if( type == c_scalar_tag_name )
		return field_t::scalar_field_type;
	else if( type == c_no_value_tag_name )
		return field_t::no_value_field_type;
	else if( type == c_scalar_vector_tag_name )
		return field_t::scalar_vector_field_type;
	else if( type == c_vector_of_tags_tag_name )
		return field_t::vector_of_tags_field_type;
	else if( type == c_custom_tag_name )
		return field_t::custom_tag_field_type;
	else
		return field_t::unknown_field_type;
}

static inline void throw_constraint_redefinition( const std::string & class_name,
	const std::string & field_name, long long line_number, long long column_number )
{
	throw cfgfile::exception_t< cfgfile::string_trait_t >( std::string( "Redefinition of "
			"constraint in class \"" ) + class_name +
		"\", field \"" + field_name +
		"\". Line " + std::to_string( line_number ) +
		", column " + std::to_string( column_number ) + "." );
}

static inline void check_constraints(
	const cfgfile::tag_t< cfgfile::string_trait_t > & c1,
	const cfgfile::tag_t< cfgfile::string_trait_t > & c2,
	const std::string & class_name, const std::string& field_name )
{
	if( c1.is_defined() && c2.is_defined() )
	{
		if( c1.line_number() == c2.line_number() )
		{
			if( c1.column_number() > c2.column_number() )
				throw_constraint_redefinition( class_name, field_name,
					c1.line_number(),
					c1.column_number() );
			else
				throw_constraint_redefinition( class_name, field_name,
					c2.line_number(),
					c2.column_number() );
		}
		else if( c1.line_number() > c2.line_number() )
			throw_constraint_redefinition( class_name, field_name,
				c1.line_number(),
				c1.column_number() );
		else
			throw_constraint_redefinition( class_name, field_name,
				c2.line_number(),
				c2.column_number() );
	}
}

field_t
tag_field_t::cfg() const
{
	field_t f;
	f.set_type( field_type_from_string( name() ) );
	f.set_name( m_name.value() );
	f.set_line_number( line_number() );
	f.set_column_number( column_number() );

	if( m_value_type.is_defined() )
		f.set_value_type( m_value_type.value() );

	if( m_is_required.is_defined() )
		f.set_required();

	if( m_default_value.is_defined() )
		f.set_default_value( m_default_value.value() );
	else if( f.type() == field_t::no_value_field_type )
		f.set_default_value( "false" );

	const tag_class_t * c = static_cast< const tag_class_t* > ( parent() );

	check_constraints( m_min_max_constraint, m_one_of_constraint,
		c->name(), m_name.value() );

	if( m_min_max_constraint.is_defined() )
		f.set_constraint( m_min_max_constraint.cfg() );
	else if( m_one_of_constraint.is_defined() )
		f.set_constraint( m_one_of_constraint.cfg() );

	return f;
}

void
tag_field_t::on_finish( const parser_info_t< cfgfile::string_trait_t > & info )
{
	switch( field_type_from_string( name() ) )
	{
		case field_t::scalar_field_type :
		case field_t::scalar_vector_field_type :
		case field_t::vector_of_tags_field_type :
		{
			if( !m_value_type.is_defined() )
				throw cfgfile::exception_t< cfgfile::string_trait_t >( std::string( "Undefined required "
						"tag \"" ) + c_value_type_tag_name +
					"\" in tag \"" + name() +
					"\". Line " + std::to_string( info.line_number() ) +
					", column " + std::to_string( info.column_number() ) + "." );
		}
			break;

		default :
			break;
	}

	cfgfile::tag_no_value_t<>::on_finish( info );
}


//
// tag_base_class_t
//

tag_base_class_t::tag_base_class_t(
	cfgfile::tag_t< cfgfile::string_trait_t > & owner,
	const std::string & name, bool is_mandatory )
	:	cfgfile::tag_scalar_t< std::string > ( owner, name, is_mandatory )
	,	m_value_type( *this, c_value_type_tag_name, false )
	,	m_name( *this, c_field_name_tag_name, false )
	,	m_min_max_constraint( *this )
	,	m_one_of_constraint( *this )
	,	m_is_required( *this, c_required_tag_name, false )
	,	m_default_value( *this, c_default_value_tag_name, false )
{
	m_constraint.add_value( c_scalar_tag_name );
	m_constraint.add_value( c_no_value_tag_name );
	m_constraint.add_value( c_scalar_vector_tag_name );

	set_constraint( &m_constraint );
}

tag_base_class_t::~tag_base_class_t()
{
}

std::string
tag_base_class_t::value_type() const
{
	return m_value_type.value();
}

std::string
tag_base_class_t::tag_name() const
{
	if( m_name.is_defined() )
		return m_name.value();
	else
		return std::string();
}

field_t
tag_base_class_t::cfg() const
{
	field_t f;
	f.set_base( true );
	f.set_type( field_type_from_string( value() ) );
	f.set_name( m_name.value() );
	f.set_line_number( line_number() );
	f.set_column_number( column_number() );

	if( m_value_type.is_defined() )
		f.set_value_type( m_value_type.value() );

	if( m_is_required.is_defined() )
		f.set_required();

	if( m_default_value.is_defined() )
		f.set_default_value( m_default_value.value() );

	const tag_class_t * c = static_cast< const tag_class_t* > ( parent() );

	check_constraints( m_min_max_constraint, m_one_of_constraint,
		c->name(), m_name.value() );

	if( m_min_max_constraint.is_defined() )
		f.set_constraint( m_min_max_constraint.cfg() );
	else if( m_one_of_constraint.is_defined() )
		f.set_constraint( m_one_of_constraint.cfg() );

	return f;
}

void
tag_base_class_t::on_finish( const parser_info_t< cfgfile::string_trait_t > & info )
{
	switch( field_type_from_string( value() ) )
	{
		case field_t::scalar_field_type :
		case field_t::scalar_vector_field_type :
		case field_t::vector_of_tags_field_type :
		{
			if( !m_value_type.is_defined() )
				throw cfgfile::exception_t< cfgfile::string_trait_t >( std::string( "Undefined required "
						"tag \"" ) + c_value_type_tag_name +
					"\" in tag \"" + c_base_class_tag_name +
					"\". Line " + std::to_string( info.line_number() ) +
					", column " + std::to_string( info.column_number() ) + "." );

			if( !m_name.is_defined() )
				throw cfgfile::exception_t< cfgfile::string_trait_t >( std::string( "Undefined required "
						"tag \"" ) + c_field_name_tag_name +
					"\" in tag \"" + c_base_class_tag_name +
					"\". Line " + std::to_string( info.line_number() ) +
					", column " + std::to_string( info.column_number() ) + "." );
		}
			break;

		default :
			break;
	}

	cfgfile::tag_scalar_t< std::string >::on_finish( info );
}


//
// tag_class_t
//

tag_class_t::tag_class_t( const std::string & name, bool is_mandatory )
	:	cfgfile::tag_scalar_t< std::string > ( name, is_mandatory )
	,	m_base_class_name( *this, c_base_class_tag_name, false )
	,	m_scalar_tags( *this, c_scalar_tag_name, false )
	,	m_no_value_tags( *this, c_no_value_tag_name, false )
	,	m_scalar_vector_tags( *this, c_scalar_vector_tag_name, false )
	,	m_vector_of_tags_tags( *this, c_vector_of_tags_tag_name, false )
	,	m_custom_tags( *this, c_custom_tag_name, false )
{
}

tag_class_t::~tag_class_t()
{
}

class_t
tag_class_t::cfg() const
{
	class_t c;
	c.set_name( value() );

	if( m_base_class_name.is_defined() )
	{
		c.set_base_name( m_base_class_name.value() );

		c.set_tag_name( m_base_class_name.tag_name() );

		if( c.base_name() != c_no_value_tag_name )
		{
			c.set_base_value_type( m_base_class_name.value_type() );

			c.add_field( m_base_class_name.cfg() );
		}
	}

	c.set_line_number( line_number() );
	c.set_column_number( column_number() );

	if( m_scalar_tags.is_defined() )
	{
		for( std::size_t i = 0; i < m_scalar_tags.size(); ++i )
			c.add_field( m_scalar_tags.at( i ).cfg() );
	}

	if( m_no_value_tags.is_defined() )
	{
		for( std::size_t i = 0; i < m_no_value_tags.size(); ++i )
			c.add_field( m_no_value_tags.at( i ).cfg() );
	}

	if( m_scalar_vector_tags.is_defined() )
	{
		for( std::size_t i = 0; i < m_scalar_vector_tags.size(); ++i )
			c.add_field( m_scalar_vector_tags.at( i ).cfg() );
	}

	if( m_vector_of_tags_tags.is_defined() )
	{
		for( std::size_t i = 0; i < m_vector_of_tags_tags.size(); ++i )
			c.add_field( m_vector_of_tags_tags.at( i ).cfg() );
	}

	if( m_custom_tags.is_defined() )
	{
		for( std::size_t i = 0; i < m_custom_tags.size(); ++i )
			c.add_field( m_custom_tags.at( i ).cfg() );
	}

	return c;
}


//
// tag_namespace_t
//

tag_namespace_t::tag_namespace_t( const std::string & name, bool is_mandatory )
	:	cfgfile::tag_scalar_t< std::string > ( name, is_mandatory )
	,	m_nested_namespaces( *this, c_namespace_tag_name, false )
	,	m_classes( *this, c_class_tag_name, false )
{
}

tag_namespace_t::~tag_namespace_t()
{
}

namespace_t
tag_namespace_t::cfg() const
{
	namespace_t n;
	n.set_name( value() );
	n.set_line_number( line_number() );
	n.set_column_number( column_number() );

	if( m_nested_namespaces.is_defined() )
	{
		for( std::size_t i = 0; i < m_nested_namespaces.size(); ++i )
			n.add_namespace( m_nested_namespaces.at( i ).cfg() );
	}

	if( m_classes.is_defined() )
	{
		for( std::size_t i = 0; i < m_classes.size(); ++i )
			n.add_class( m_classes.at( i ).cfg() );
	}

	return n;
}


//
// tag_model_t
//

tag_model_t::tag_model_t()
	:	cfgfile::tag_scalar_t< std::string > ( c_main_cfg_tag_name, true )
	,	m_root_namespace( *this, c_namespace_tag_name, false )
	,	m_root_classes( *this, c_class_tag_name, false )
	,	m_global_includes( *this, c_global_include_tag_name, false )
	,	m_relative_includes( *this, c_relative_include_tag_name, false )
{
}

tag_model_t::~tag_model_t()
{
}

model_t
tag_model_t::cfg() const
{
	model_t m;
	m.set_include_guard( value() );

	if( m_root_namespace.is_defined() )
	{
		for( std::size_t i = 0; i < m_root_namespace.size(); ++i )
			m.root_namespace().add_namespace( m_root_namespace.at( i ).cfg() );
	}

	if( m_root_classes.is_defined() )
	{
		for( std::size_t i = 0; i < m_root_classes.size(); ++i )
			m.root_namespace().add_class( m_root_classes.at( i ).cfg() );
	}

	if( m_global_includes.is_defined() )
	{
		for( std::size_t i = 0; i < m_global_includes.size(); ++i )
			m.add_global_include( m_global_includes.at( i ).value() );
	}

	if( m_relative_includes.is_defined() )
	{
		for( std::size_t i = 0; i < m_relative_includes.size(); ++i )
			m.add_relative_include( m_relative_includes.at( i ).value() );
	}

	return m;
}

} /* namespace cfg */

} /* namespace generator */

} /* namespace cfgfile */
