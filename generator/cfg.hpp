
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

#ifndef CFGFILE__GENERATOR__CFG__CFG_HPP__INCLUDED
#define CFGFILE__GENERATOR__CFG__CFG_HPP__INCLUDED

// cfgfile include.
#include <cfgfile/all.hpp>

// C++ include.
#include <string>
#include <map>


namespace cfgfile {

namespace generator {

namespace cfg {

//! Main cfg tag's name.
static const std::string c_main_cfg_tag_name( "forGeneration" );
//! Namespace tag's name.
static const std::string c_namespace_tag_name( "namespace" );
//! Class tag's name.
static const std::string c_class_tag_name( "class" );
//! Base class tag's name.
static const std::string c_base_class_tag_name( "base" );
//! Name of field tag's name.
static const std::string c_field_name_tag_name( "name" );
//! Min-max constraint tag's name.
static const std::string c_min_max_constraint_tag_name( "minMaxConstraint" );
//! One-of constraint tag's name.
static const std::string c_one_of_constraint_tag_name( "oneOfConstraint" );
//! Min tag's name.
static const std::string c_min_tag_name( "min" );
//! Max tag's name.
static const std::string c_max_tag_name( "max" );
//! Scalar field tag's name.
static const std::string c_scalar_tag_name( "tagScalar" );
//! No-value field tag's name.
static const std::string c_no_value_tag_name( "tagNoValue" );
//! Scalar vector field tag's name.
static const std::string c_scalar_vector_tag_name( "tagScalarVector" );
//! Vector of tags field tag's name.
static const std::string c_vector_of_tags_tag_name( "tagVectorOfTags" );
//! Custom tag field tag's name.
static const std::string c_custom_tag_name( "tag" );
//! Namespace separator.
static const std::string c_namespace_separator( "::" );
//! Global include tag's name.
static const std::string c_global_include_tag_name( "globalInclude" );
//! Relative include tag's name.
static const std::string c_relative_include_tag_name( "relativeInclude" );
//! Required tag name.
static const std::string c_required_tag_name( "required" );
//! Default value tag name.
static const std::string c_default_value_tag_name( "defaultValue" );
//! Value type tag name.
static const std::string c_value_type_tag_name( "valueType" );


//
// constraint_base_t
//

//! Base class for constraints.
class constraint_base_t {
public:
	constraint_base_t();
	virtual ~constraint_base_t();

	//! Type of the constraint.
	enum constraint_type_t {
		//! Undefined.
		undefined_constraint_type = 0,
		//! Min-max.
		min_max_constraint_type = 1,
		//! One of.
		one_of_constraint_type = 2
	}; // enum constraint_type_t

	//! \return Constraint type.
	virtual constraint_type_t type() const = 0;
}; // class constraint_base_t


//
// min_max_constraint_t
//

//! Min-max constraint.
class min_max_constraint_t
	:	public constraint_base_t
{
public:
	min_max_constraint_t( const std::string & minValue,
		const std::string & maxValue );
	virtual ~min_max_constraint_t();

	//! \return Constraint type.
	virtual constraint_type_t type() const;

	//! \return Min value.
	const std::string & min() const;
	//! \return Max value.
	const std::string & max() const;

private:
	//! Min value.
	std::string m_min;
	//! Max value.
	std::string m_max;
}; // class min_max_constraint_t


//
// one_of_constraint_t
//

//! One of constraint.
class one_of_constraint_t
	:	public constraint_base_t
{
public:
	explicit one_of_constraint_t( const std::vector< std::string > & values );
	virtual ~one_of_constraint_t();

	//! \return Constraint type.
	virtual constraint_type_t type() const;

	//! \return List of values.
	const std::vector< std::string > & values() const;

private:
	//! List of values.
	std::vector< std::string > m_values;
}; // class one_of_constraint_t


//
// field_t
//

//! This is data field in class.
class field_t {
public:
	//! Type of the field.
	enum field_type_t {
		//! Unknown.
		unknown_field_type = 0,
		//! Scalar.
		scalar_field_type  = 1,
		//! No value.
		no_value_field_type = 2,
		//! Scalar vector.
		scalar_vector_field_type = 3,
		//! Vector of tags.
		vector_of_tags_field_type = 4,
		//! Custome tag.
		custom_tag_field_type = 5
	}; // enum field_type_t

	field_t();
	~field_t();

	bool operator == ( const std::string & n ) const;

	//! \return Type of the field.
	field_type_t type() const;
	//! Set type of the field.
	void set_type( field_type_t t );

	//! \return Type of the field's value.
	const std::string & value_type() const;
	//! Set type of the field's value.
	void set_value_type( const std::string & t );

	//! \return Name of the field.
	const std::string & name() const;
	//! Set name of the field.
	void set_name( const std::string & n );

	//! \return Is constraint null.
	bool is_constraint_null() const;

	//! \return Constraint.
	const std::shared_ptr< constraint_base_t > & constraint() const;
	//! Set constraint.
	void set_constraint( const std::shared_ptr< constraint_base_t > & c );

	//! \return Line number.
	long long line_number() const;
	//! Set line number.
	void set_line_number( long long num );

	//! \return Column number.
	long long column_number() const;
	//! Set column number.
	void set_column_number( long long num );

	//! \return Is field required?
	bool is_required() const;
	//! Set requreid.
	void set_required( bool on = true );

	//! \return Default value for the field.
	const std::string & default_value() const;
	//! Set default value.
	void set_default_value( const std::string & value );

	//! \return Is this field the base of class?
	bool is_base() const;
	//! Set that this field is the base of class.
	void set_base( bool on = true );

private:
	//! Type
	field_type_t m_type;
	//! Value's type.
	std::string m_value_type;
	//! Name.
	std::string m_name;
	//! Constraint.
	std::shared_ptr< constraint_base_t > m_constraint;
	//! Line number.
	long long m_line_number;
	//! Column number.
	long long m_column_number;
	//! Is required?
	bool m_is_required;
	//! Default value.
	std::string m_default_value;
	//! Is base of class?
	bool m_is_base;
}; // class field_t


class namespace_t;


//
// class_t
//

//! This is data structure uses to store class.
class class_t {
public:
	class_t();
	~class_t();

	class_t( const class_t & other );
	class_t & operator = ( const class_t & other );

	bool operator == ( const std::string & n ) const;

	//! \return Name of the class.
	const std::string & name() const;
	//! Set name of the class.
	void set_name( const std::string & n );

	//! \return Tag name if defined.
	const std::string & tag_name() const;
	//! Set tag name.
	void set_tag_name( const std::string & n );

	//! \return Base class name.
	const std::string & base_name() const;
	//! Set base class name.
	void set_base_name( const std::string & n );

	//! \return Base value type.
	const std::string & base_value_type() const;
	//! Set base value type.
	void set_base_value_type( const std::string & t );

	//! \return all fields.
	const std::vector< field_t > & fields() const;

	//! \return Field with the given name.
	const field_t * field_by_name( const std::string & n ) const;
	//! \return Field with the given name.
	field_t * field_by_name( const std::string & n );

	//! Add field.
	void add_field( const field_t & f );

	//! \return Line number.
	long long line_number() const;
	//! Set line number.
	void set_line_number( long long num );

	//! \return Column number.
	long long column_number() const;
	//! Set column number.
	void set_column_number( long long num );

	//! \return Index.
	unsigned long long index() const;

	//! \return Parent namespace.
	const namespace_t * parent_namespace() const;

protected:
	friend class model_t;

	friend void extract_and_bind_all_classes( const namespace_t & root,
		std::vector< const class_t* > & data );

	//! Set parent namespace.
	void set_parent_namespace( const namespace_t * p ) const;

	//! Set index.
	void set_index( unsigned long long i ) const;

private:
	//! Name of the class.
	std::string m_name;
	//! Tag name if defined.
	std::string m_tag_name;
	//! Base class name.
	std::string m_base_name;
	//! Base value type.
	std::string m_base_value_type;
	//! Fields.
	std::vector< field_t > m_fields;
	//! Line number.
	long long m_line_number;
	//! Column number.
	long long m_column_number;

	//! Index.
	mutable unsigned long long m_index;
	//! Parent namespace.
	mutable const namespace_t * m_parent;
}; // class class_t


//
// namespace_t
//

//! This data structure uses to store namespaces.
class namespace_t {
public:
	namespace_t();
	~namespace_t();

	namespace_t( const namespace_t & other );
	namespace_t & operator = ( const namespace_t & other );

	//! \return Name.
	const std::string & name() const;
	//! Set name.
	void set_name( const std::string & n );

	//! \return All nested namespaces.
	const std::vector< namespace_t > & all_nested() const;

	//! \return Nested namespace with the given name.
	std::vector< const namespace_t* > nested( const std::string & n ) const;
	//! \return Nested namespace with the given name.
	std::vector< namespace_t* > nested( const std::string & n );

	//! Add namespace.
	void add_namespace( const namespace_t & n );

	//! \return all classes.
	const std::vector< class_t > & classes() const;

	//! \return Class with the given name.
	const class_t * class_by_name( const std::string & n ) const;
	//! \return Class with the given name.
	class_t * class_by_name( const std::string & n );

	//! Add class.
	void add_class( const class_t & c );

	//! \return Line number.
	long long line_number() const;
	//! Set line number.
	void set_line_number( long long num );

	//! \return Column number.
	long long column_number() const;
	//! Set column number.
	void set_column_number( long long num );

	//! \return Parent namespace.
	const namespace_t * parent_namespace() const;

protected:
	friend void extract_and_bind_all_classes( const namespace_t & root,
		std::vector< const class_t* > & data );

	//! Set parent namespace.
	void set_parent_namespace( const namespace_t * p ) const;

private:
	//! Namespace name.
	std::string m_name;
	//! Nested namespaces.
	std::vector< namespace_t > m_nested_namespaces;
	//! Classes in this namespace.
	std::vector< class_t > m_classes;
	//! Line number.
	long long m_line_number;
	//! Column number.
	long long m_column_number;

	//! Parent namespace.
	mutable const namespace_t * m_parent;
}; // class namespace_t


typedef const class_t* const_class_ptr_t;


//
// model_t
//

//! This is model of the configuration uses to generate C++ code.
class model_t {
public:
	model_t();
	~model_t();

	model_t( const model_t & other );
	model_t & operator = ( const model_t & other );

	//! \return Root namespace.
	const namespace_t & root_namespace() const;
	//! \return Root namespace.
	namespace_t & root_namespace();
	//! Set root namespace.
	void set_root_namespace( const namespace_t & n );

	//! \return List of global includes.
	const std::vector< std::string > & global_includes() const;
	//! Set list of global includes.
	void set_global_includes( const std::vector< std::string > & inc );
	//! Add global include.
	void add_global_include( const std::string & inc );

	//! \return List of relative includes.
	const std::vector< std::string > & relative_includes() const;
	//! Set list of relative includes.
	void set_relative_includes( const std::vector< std::string > & inc );
	//! Add relative include.
	void add_relative_include( const std::string & inc );

	//! Prepare model. After call of this method modifications not
	//! allowed. If you modified something you have to re-call
	//! this method again to have correct links, etc... Even copying
	//! will broke links.
	void prepare();

	//! Check model. \throw cfgfile::Exception on error.
	//! Model must be prepared before calling this method.
	void check() const;

	//! Find next class.
	const_class_ptr_t next_class( unsigned long long index ) const;

	//! \return Is included something?
	bool is_included() const;

	//! \return Macro for include guard.
	const std::string & include_guard() const;
	//! Set include guard.
	void set_include_guard( const std::string & guard );

private:
	//! Check class.
	void check_class( const class_t & c,
		std::vector< std::string > & prev_defined_classes,
		bool included ) const;

private:
	//! Root namespace, i.e. ::
	namespace_t m_root;
	//! List of global includes.
	std::vector< std::string > m_global_includes;
	//! List of relative includes.
	std::vector< std::string > m_relative_includes;
	//! Include guard.
	std::string m_include_guard;

	//! Prepared data.
	std::map< unsigned long long, const_class_ptr_t > m_indexes;
}; // class model_t


//
// tag_min_max_constraint_t
//

//! Tag for min_max_constraint_t.
class tag_min_max_constraint_t
	:	public cfgfile::tag_no_value_t<>
{
public:
	explicit tag_min_max_constraint_t(
		cfgfile::tag_t< cfgfile::string_trait_t > & owner );
	~tag_min_max_constraint_t();

	//! \return Constraint.
	std::shared_ptr< min_max_constraint_t > cfg() const;

private:
	//! Min value.
	cfgfile::tag_scalar_t< std::string > m_min;
	//! Max value.
	cfgfile::tag_scalar_t< std::string > m_max;
}; // class tag_min_max_constraint_t


//
// tag_one_of_constraint_t
//

//! Tag for one_of_constraint_t.
class tag_one_of_constraint_t
	:	public cfgfile::tag_scalar_vector_t< std::string >
{
public:
	explicit tag_one_of_constraint_t(
		cfgfile::tag_t< cfgfile::string_trait_t > & owner );
	~tag_one_of_constraint_t();

	//! \return Constraint.
	std::shared_ptr< one_of_constraint_t > cfg() const;
}; // class tag_one_of_constraint_t


//
// tag_field_t
//

//! Tag for Field.
class tag_field_t
	:	public cfgfile::tag_no_value_t<>
{
public:
	tag_field_t( const std::string & name, bool is_mandatory );
	~tag_field_t();

	//! \return Field.
	field_t cfg() const;

	//! Called when tag parsing finished.
	void on_finish( const parser_info_t< cfgfile::string_trait_t > & info );

private:
	//! Name.
	cfgfile::tag_scalar_t< std::string > m_name;
	//! Value type.
	cfgfile::tag_scalar_t< std::string > m_value_type;
	//! Min-max constraint.
	tag_min_max_constraint_t m_min_max_constraint;
	//! One of constraint.
	tag_one_of_constraint_t m_one_of_constraint;
	//! Is required?
	cfgfile::tag_no_value_t<> m_is_required;
	//! Default value.
	cfgfile::tag_scalar_t< std::string > m_default_value;
}; // class tag_field_t


//
// tag_base_class_t
//

class tag_base_class_t
	:	public cfgfile::tag_scalar_t< std::string >
{
public:
	tag_base_class_t( cfgfile::tag_t< cfgfile::string_trait_t > & owner,
		const std::string & name, bool is_mandatory );
	~tag_base_class_t();

	//! \return Value type.
	std::string value_type() const;

	//! \return Tag name.
	std::string tag_name() const;

	//! \return Field.
	field_t cfg() const;

	//! Called when tag parsing finished.
	void on_finish( const parser_info_t< cfgfile::string_trait_t > & info );

private:
	//! Value type.
	cfgfile::tag_scalar_t< std::string > m_value_type;
	//! Constraint.
	cfgfile::constraint_one_of_t< std::string > m_constraint;
	//! Name.
	cfgfile::tag_scalar_t< std::string > m_name;
	//! Min-max constraint.
	tag_min_max_constraint_t m_min_max_constraint;
	//! One of constraint.
	tag_one_of_constraint_t m_one_of_constraint;
	//! Is required?
	cfgfile::tag_no_value_t<> m_is_required;
	//! Default value.
	cfgfile::tag_scalar_t< std::string > m_default_value;
}; // class tag_base_class_t


//
// tag_class_t
//

//! Tag for Class.
class tag_class_t
	:	public cfgfile::tag_scalar_t< std::string >
{
public:
	tag_class_t( const std::string & name, bool is_mandatory );
	~tag_class_t();

	//! \return Class.
	class_t cfg() const;

private:
	//! Base class name.
	tag_base_class_t m_base_class_name;
	//! Scalar tags.
	cfgfile::tag_vector_of_tags_t< tag_field_t > m_scalar_tags;
	//! No-value tags.
	cfgfile::tag_vector_of_tags_t< tag_field_t > m_no_value_tags;
	//! Scalar-vector tags.
	cfgfile::tag_vector_of_tags_t< tag_field_t > m_scalar_vector_tags;
	//! Vector-of-tags tags.
	cfgfile::tag_vector_of_tags_t< tag_field_t > m_vector_of_tags_tags;
	//! Custom tags.
	cfgfile::tag_vector_of_tags_t< tag_field_t > m_custom_tags;
}; // class tag_scalar_t


//
// tag_namespace_t
//

//! Tag for Namespace.
class tag_namespace_t
	:	public cfgfile::tag_scalar_t< std::string >
{
public:
	tag_namespace_t( const std::string & name, bool is_mandatory );
	~tag_namespace_t();

	//! \return Namespace.
	namespace_t cfg() const;

private:
	//! Nested namespaces.
	cfgfile::tag_vector_of_tags_t< tag_namespace_t > m_nested_namespaces;
	//! Classes.
	cfgfile::tag_vector_of_tags_t< tag_class_t > m_classes;
}; // class tag_namespace_t


//
// tag_model_t
//

//! Tag for Model.
class tag_model_t
	:	public cfgfile::tag_scalar_t< std::string >
{
public:
	tag_model_t();
	~tag_model_t();

	//! \return Model.
	model_t cfg() const;

private:
	//! Root namespace.
	cfgfile::tag_vector_of_tags_t< tag_namespace_t > m_root_namespace;
	//! Classes in root namespace.
	cfgfile::tag_vector_of_tags_t< tag_class_t > m_root_classes;
	//! Global includes.
	cfgfile::tag_vector_of_tags_t< cfgfile::tag_scalar_t< std::string > >
		m_global_includes;
	//! Relative includes.
	cfgfile::tag_vector_of_tags_t< cfgfile::tag_scalar_t< std::string > >
		m_relative_includes;
}; // class tag_model_t

} /* namespace cfg */

} /* namespace generator */

} /* namespace cfgfile */

#endif // CFGFILE__GENERATOR__CFG__CFG_HPP__INCLUDED
