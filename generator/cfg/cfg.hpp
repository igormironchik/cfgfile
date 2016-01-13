
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

#ifndef QTCONFFILE__GENERATOR__CFG__CFG_HPP__INCLUDED
#define QTCONFFILE__GENERATOR__CFG__CFG_HPP__INCLUDED

// QtConfFile include.
#include <QtConfFile/TagNoValue>
#include <QtConfFile/TagScalar>
#include <QtConfFile/TagVectorOfTags>
#include <QtConfFile/TagScalarVector>

// Qt include.
#include <QList>
#include <QSharedPointer>
#include <QMap>


namespace QtConfFile {

namespace Generator {

namespace Cfg {

//! Main cfg tag's name.
static const QString c_mainCfgTagName = QLatin1String( "forGeneration" );
//! Namespace tag's name.
static const QString c_namespaceTagName = QLatin1String( "namespace" );
//! Class tag's name.
static const QString c_classTagName = QLatin1String( "class" );
//! Base class tag's name.
static const QString c_baseClassTagName = QLatin1String( "base" );
//! Name of field tag's name.
static const QString c_fieldNameTagName = QLatin1String( "name" );
//! Min-max constraint tag's name.
static const QString c_minMaxConstraintTagName =
	QLatin1String( "minMaxConstraint" );
//! One-of constraint tag's name.
static const QString c_oneOfConstraintTagName =
	QLatin1String( "oneOfConstraint" );
//! Min tag's name.
static const QString c_minTagName = QLatin1String( "min" );
//! Max tag's name.
static const QString c_maxTagName = QLatin1String( "max" );
//! Scalar field tag's name.
static const QString c_scalarTagName = QLatin1String( "tagScalar" );
//! No-value field tag's name.
static const QString c_noValueTagName = QLatin1String( "tagNoValue" );
//! Scalar vector field tag's name.
static const QString c_scalarVectorTagName = QLatin1String( "tagScalarVector" );
//! Vector of tags field tag's name.
static const QString c_vectorOfTagsTagName = QLatin1String( "tagVectorOfTags" );
//! Custom tag field tag's name.
static const QString c_customTagName = QLatin1String( "tag" );
//! Namespace separator.
static const QString c_namespaceSeparator = QLatin1String( "::" );
//! Global include tag's name.
static const QString c_globalIncludeTagName = QLatin1String( "globalInclude" );
//! Relative include tag's name.
static const QString c_relativeIncludeTagName =
	QLatin1String( "relativeInclude" );
//! Required tag name.
static const QString c_requiredTagName = QLatin1String( "required" );
//! Default value tag name.
static const QString c_defaultValueTagName = QLatin1String( "defaultValue" );
//! Value type tag name.
static const QString c_valueTypeTagName = QLatin1String( "valueType" );


//
// ConstraintBase
//

//! Base class for constraints.
class ConstraintBase {
public:
	ConstraintBase();
	virtual ~ConstraintBase();

	//! Type of the constraint.
	enum ConstraintType {
		//! Undefined.
		UndefinedConstraintType = 0,
		//! Min-max.
		MinMaxConstraintType = 1,
		//! One of.
		OneOfConstraintType = 2
	}; // enum ConstraintType

	//! \return Constraint type.
	virtual ConstraintType type() const = 0;
}; // class ConstraintBase


//
// MinMaxConstraint
//

//! Min-max constraint.
class MinMaxConstraint
	:	public ConstraintBase
{
public:
	MinMaxConstraint( const QString & minValue,
		const QString & maxValue );
	virtual ~MinMaxConstraint();

	//! \return Constraint type.
	virtual ConstraintType type() const;

	//! \return Min value.
	const QString & min() const;
	//! \return Max value.
	const QString & max() const;

private:
	//! Min value.
	QString m_min;
	//! Max value.
	QString m_max;
}; // class MinMaxConstraint


//
// OneOfConstraint
//

//! One of constraint.
class OneOfConstraint
	:	public ConstraintBase
{
public:
	explicit OneOfConstraint( const QStringList & valuesList );
	virtual ~OneOfConstraint();

	//! \return Constraint type.
	virtual ConstraintType type() const;

	//! \return List of values.
	const QStringList & values() const;

private:
	//! List of values.
	QStringList m_values;
}; // class OneOfConstraint


//
// Field
//

//! This is data field in class.
class Field {
public:
	//! Type of the field.
	enum FieldType {
		//! Unknown.
		UnknownFieldType = 0,
		//! Scalar.
		ScalarFieldType  = 1,
		//! No value.
		NoValueFieldType = 2,
		//! Scalar vector.
		ScalarVectorFieldType = 3,
		//! Vector of tags.
		VectorOfTagsFieldType = 4,
		//! Custome tag.
		CustomTagFieldType = 5
	}; // enum FieldType

	Field();
	~Field();

	Field( const Field & other );
	Field & operator = ( const Field & other );

	bool operator == ( const QString & n ) const;

	//! \return Type of the field.
	FieldType type() const;
	//! Set type of the field.
	void setType( FieldType t );

	//! \return Type of the field's value.
	const QString & valueType() const;
	//! Set type of the field's value.
	void setValueType( const QString & t );

	//! \return Name of the field.
	const QString & name() const;
	//! Set name of the field.
	void setName( const QString & n );

	//! \return Is constraint null.
	bool isConstraintNull() const;

	//! \return Constraint.
	const QSharedPointer< ConstraintBase > & constraint() const;
	//! Set constraint.
	void setConstraint( const QSharedPointer< ConstraintBase > & c );

	//! \return Line number.
	qint64 lineNumber() const;
	//! Set line number.
	void setLineNumber( qint64 num );

	//! \return Column number.
	qint64 columnNumber() const;
	//! Set column number.
	void setColumnNumber( qint64 num );

	//! \return Is field required?
	bool isRequired() const;
	//! Set requreid.
	void setRequired( bool on = true );

	//! \return Default value for the field.
	const QString & defaultValue() const;
	//! Set default value.
	void setDefaultValue( const QString & value );

private:
	//! Type
	FieldType m_type;
	//! Value's type.
	QString m_valueType;
	//! Name.
	QString m_name;
	//! Constraint.
	QSharedPointer< ConstraintBase > m_constraint;
	//! Line number.
	qint64 m_lineNumber;
	//! Column number.
	qint64 m_columnNumber;
	//! Is required?
	bool m_isRequired;
	//! Default value.
	QString m_defaultValue;
}; // class Field


class Namespace;


//
// Class
//

//! This is data structure uses to store class.
class Class {
public:
	Class();
	~Class();

	Class( const Class & other );
	Class & operator = ( const Class & other );

	bool operator == ( const QString & n ) const;

	//! \return Name of the class.
	const QString & name() const;
	//! Set name of the class.
	void setName( const QString & n );

	//! \return Base class name.
	const QString & baseName() const;
	//! Set base class name.
	void setBaseName( const QString & n );

	//! \return Base value type.
	const QString & baseValueType() const;
	//! Set base value type.
	void setBaseValueType( const QString & t );

	//! \return all fields.
	const QList< Field > & fields() const;

	//! \return Field with the given name.
	const Field * fieldByName( const QString & n ) const;
	//! \return Field with the given name.
	Field * fieldByName( const QString & n );

	//! Add field.
	void addField( const Field & f );

	//! \return Line number.
	qint64 lineNumber() const;
	//! Set line number.
	void setLineNumber( qint64 num );

	//! \return Column number.
	qint64 columnNumber() const;
	//! Set column number.
	void setColumnNumber( qint64 num );

	//! \return Index.
	quint64 index() const;

	//! \return Parent namespace.
	const Namespace * parentNamespace() const;

protected:
	friend class Model;

	friend void extractAndBindAllClasses( const Namespace & root,
		QList< const Class* > & data );

	//! Set parent namespace.
	void setParentNamespace( const Namespace * p ) const;

	//! Set index.
	void setIndex( quint64 i ) const;

private:
	//! Name of the class.
	QString m_name;
	//! Base class name.
	QString m_baseName;
	//! Base value type.
	QString m_baseValueType;
	//! Fields.
	QList< Field > m_fields;
	//! Line number.
	qint64 m_lineNumber;
	//! Column number.
	qint64 m_columnNumber;

	//! Index.
	mutable quint64 m_index;
	//! Parent namespace.
	mutable const Namespace * m_parent;
}; // class Class


//
// Namespace
//

//! This data structure uses to store namespaces.
class Namespace {
public:
	Namespace();
	~Namespace();

	Namespace( const Namespace & other );
	Namespace & operator = ( const Namespace & other );

	//! \return Name.
	const QString & name() const;
	//! Set name.
	void setName( const QString & n );

	//! \return All nested namespaces.
	const QList< Namespace > & allNested() const;

	//! \return Nested namespace with the given name.
	QList< const Namespace* > nested( const QString & n ) const;
	//! \return Nested namespace with the given name.
	QList< Namespace* > nested( const QString & n );

	//! Add namespace.
	void addNamespace( const Namespace & n );

	//! \return all classes.
	const QList< Class > & classes() const;

	//! \return Class with the given name.
	const Class * classByName( const QString & n ) const;
	//! \return Class with the given name.
	Class * classByName( const QString & n );

	//! Add class.
	void addClass( const Class & c );

	//! \return Line number.
	qint64 lineNumber() const;
	//! Set line number.
	void setLineNumber( qint64 num );

	//! \return Column number.
	qint64 columnNumber() const;
	//! Set column number.
	void setColumnNumber( qint64 num );

	//! \return Parent namespace.
	const Namespace * parentNamespace() const;

protected:
	friend void extractAndBindAllClasses( const Namespace & root,
		QList< const Class* > & data );

	//! Set parent namespace.
	void setParentNamespace( const Namespace * p ) const;

private:
	//! Namespace name.
	QString m_name;
	//! Nested namespaces.
	QList< Namespace > m_nestedNamespaces;
	//! Classes in this namespace.
	QList< Class > m_classes;
	//! Line number.
	qint64 m_lineNumber;
	//! Column number.
	qint64 m_columnNumber;

	//! Parent namespace.
	mutable const Namespace * m_parent;
}; // class Namespace


typedef const Class* ConstClassPointer;


//
// Model
//

//! This is model of the configuration uses to generate C++ code.
class Model {
public:
	Model();
	~Model();

	Model( const Model & other );
	Model & operator = ( const Model & other );

	//! \return Root namespace.
	const Namespace & rootNamespace() const;
	//! \return Root namespace.
	Namespace & rootNamespace();
	//! Set root namespace.
	void setRootNamespace( const Namespace & n );

	//! \return List of global includes.
	const QStringList & globalIncludes() const;
	//! Set list of global includes.
	void setGlobalIncludes( const QStringList & inc );
	//! Add global include.
	void addGlobalInclude( const QString & inc );

	//! \return List of relative includes.
	const QStringList & relativeIncludes() const;
	//! Set list of relative includes.
	void setRelativeIncludes( const QStringList & inc );
	//! Add relative include.
	void addRelativeInclude( const QString & inc );

	//! Prepare model. After call of this method modifications not
	//! allowed. If you modified something you have to re-call
	//! this method again to have correct links, etc... Even copying
	//! will broke links.
	void prepare();

	//! Check model. \throw QtConfFile::Exception on error.
	//! Model must be prepared before calling this method.
	void check() const;

	//! Find next class.
	ConstClassPointer nextClass( quint64 index ) const;

	//! \return Is included something?
	bool isIncluded() const;

	//! \return Macro for include guard.
	const QString & includeGuard() const;
	//! Set include guard.
	void setIncludeGuard( const QString & guard );

private:
	//! Check class.
	void checkClass( const Class & c,
		QStringList & prevDefinedClasses,
		bool included ) const;

private:
	//! Root namespace, i.e. ::
	Namespace m_root;
	//! List of global includes.
	QStringList m_globalIncludes;
	//! List of relative includes.
	QStringList m_relativeIncludes;
	//! Include guard.
	QString m_includeGuard;

	//! Prepared data.
	QMap< quint64, ConstClassPointer > m_indexes;
}; // class Model


//
// TagMinMaxConstraint
//

//! Tag for MinMaxConstraint.
class TagMinMaxConstraint
	:	public QtConfFile::TagNoValue
{
public:
	explicit TagMinMaxConstraint( QtConfFile::Tag & owner );
	~TagMinMaxConstraint();

	//! \return Constraint.
	QSharedPointer< MinMaxConstraint > cfg() const;

private:
	//! Min value.
	QtConfFile::TagScalar< QString > m_min;
	//! Max value.
	QtConfFile::TagScalar< QString > m_max;
}; // class TagMinMaxConstraint


//
// TagOneOfConstraint
//

//! Tag for OneOfConstraint.
class TagOneOfConstraint
	:	public QtConfFile::TagScalarVector< QString >
{
public:
	explicit TagOneOfConstraint( QtConfFile::Tag & owner );
	~TagOneOfConstraint();

	//! \return Constraint.
	QSharedPointer< OneOfConstraint > cfg() const;
}; // class TagOneOfConstraint


//
// TagField
//

//! Tag for Field.
class TagField
	:	public QtConfFile::TagNoValue
{
public:
	TagField( const QString & name, bool isMandatory );
	~TagField();

	//! \return Field.
	Field cfg() const;

	//! Called when tag parsing finished.
	void onFinish( const ParserInfo & info );

private:
	//! Name.
	QtConfFile::TagScalar< QString > m_name;
	//! Value type.
	QtConfFile::TagScalar< QString > m_valueType;
	//! Min-max constraint.
	TagMinMaxConstraint m_minMaxConstraint;
	//! One of constraint.
	TagOneOfConstraint m_oneOfConstraint;
	//! Is required?
	QtConfFile::TagNoValue m_isRequired;
	//! Default value.
	QtConfFile::TagScalar< QString > m_defaultValue;
}; // class TagField


//
// TagBaseClass
//

class TagBaseClass
	:	public QtConfFile::TagScalar< QString >
{
public:
	TagBaseClass( QtConfFile::Tag & owner, const QString & name,
		bool isMandatory );
	~TagBaseClass();

	//! \return Value type.
	QString valueType() const;

	//! Called when tag parsing finished.
	void onFinish( const ParserInfo & info );

private:
	//! Value type.
	QtConfFile::TagScalar< QString > m_valueType;
}; // class TagBaseClass


//
// TagClass
//

//! Tag for Class.
class TagClass
	:	public QtConfFile::TagScalar< QString >
{
public:
	TagClass( const QString & name, bool isMandatory );
	~TagClass();

	//! \return Class.
	Class cfg() const;

private:
	//! Base class name.
	TagBaseClass m_baseClassName;
	//! Scalar tags.
	QtConfFile::TagVectorOfTags< TagField > m_scalarTags;
	//! No-value tags.
	QtConfFile::TagVectorOfTags< TagField > m_noValueTags;
	//! Scalar-vector tags.
	QtConfFile::TagVectorOfTags< TagField > m_scalarVectorTags;
	//! Vector-of-tags tags.
	QtConfFile::TagVectorOfTags< TagField > m_vectorOfTagsTags;
	//! Custom tags.
	QtConfFile::TagVectorOfTags< TagField > m_customTags;
}; // class TagScalar


//
// TagNamespace
//

//! Tag for Namespace.
class TagNamespace
	:	public QtConfFile::TagScalar< QString >
{
public:
	TagNamespace( const QString & name, bool isMandatory );
	~TagNamespace();

	//! \return Namespace.
	Namespace cfg() const;

private:
	//! Nested namespaces.
	QtConfFile::TagVectorOfTags< TagNamespace > m_nestedNamespaces;
	//! Classes.
	QtConfFile::TagVectorOfTags< TagClass > m_classes;
}; // class TagNamespace


//
// TagModel
//

//! Tag for Model.
class TagModel
	:	public QtConfFile::TagScalar< QString >
{
public:
	TagModel();
	~TagModel();

	//! \return Model.
	Model cfg() const;

private:
	//! Root namespace.
	QtConfFile::TagVectorOfTags< TagNamespace > m_rootNamespace;
	//! Classes in root namespace.
	QtConfFile::TagVectorOfTags< TagClass > m_rootClasses;
	//! Global includes.
	QtConfFile::TagVectorOfTags< QtConfFile::TagScalar< QString > >
		m_globalIncludes;
	//! Relative includes.
	QtConfFile::TagVectorOfTags< QtConfFile::TagScalar< QString > >
		m_relativeIncludes;
}; // class TagModel

} /* namespace Cfg */

} /* namespace Generator */

} /* namespace QtConfFile */

#endif // QTCONFFILE__GENERATOR__CFG__CFG_HPP__INCLUDED
