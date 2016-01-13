
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

// Generator cfg include.
#include "cfg.hpp"

// QtConfFile include.
#include <QtConfFile/Exceptions>

// C++ include.
#include <algorithm>


namespace QtConfFile {

namespace Generator {

namespace Cfg {

//
// ConstraintBase
//

ConstraintBase::ConstraintBase()
{
}

ConstraintBase::~ConstraintBase()
{
}


//
// MinMaxConstraint
//

MinMaxConstraint::MinMaxConstraint( const QString & minValue,
	const QString & maxValue )
	:	m_min( minValue )
	,	m_max( maxValue )
{
}

MinMaxConstraint::~MinMaxConstraint()
{
}

ConstraintBase::ConstraintType
MinMaxConstraint::type() const
{
	return MinMaxConstraintType;
}

const QString &
MinMaxConstraint::min() const
{
	return m_min;
}

const QString &
MinMaxConstraint::max() const
{
	return m_max;
}


//
// OneOfConstraint
//

OneOfConstraint::OneOfConstraint( const QStringList & valuesList )
	:	m_values( valuesList )
{
}

OneOfConstraint::~OneOfConstraint()
{

}

ConstraintBase::ConstraintType
OneOfConstraint::type() const
{
	return OneOfConstraintType;
}

const QStringList &
OneOfConstraint::values() const
{
	return m_values;
}


//
// Field
//

Field::Field()
	:	m_lineNumber( -1 )
	,	m_columnNumber( -1 )
	,	m_isRequired( false )
{
}

Field::~Field()
{
}

Field::Field( const Field & other )
	:	m_type( other.type() )
	,	m_valueType( other.valueType() )
	,	m_name( other.name() )
	,	m_constraint( other.constraint() )
	,	m_lineNumber( other.lineNumber() )
	,	m_columnNumber( other.columnNumber() )
	,	m_isRequired( other.isRequired() )
	,	m_defaultValue( other.defaultValue() )
{
}

Field &
Field::operator = ( const Field & other )
{
	if( this != & other )
	{
		m_type = other.type();
		m_valueType = other.valueType();
		m_name = other.name();
		m_constraint = other.constraint();
		m_lineNumber = other.lineNumber();
		m_columnNumber = other.columnNumber();
		m_isRequired = other.isRequired();
		m_defaultValue = other.defaultValue();
	}

	return *this;
}

bool
Field::operator == ( const QString & n ) const
{
	return ( name() == n );
}

Field::FieldType
Field::type() const
{
	return m_type;
}

void
Field::setType( FieldType t )
{
	m_type = t;
}

const QString &
Field::valueType() const
{
	return m_valueType;
}

void
Field::setValueType( const QString & t )
{
	m_valueType = t;
}

const QString &
Field::name() const
{
	return m_name;
}

void
Field::setName( const QString & n )
{
	m_name = n;
}

bool
Field::isConstraintNull() const
{
	return m_constraint.isNull();
}

const QSharedPointer< ConstraintBase > &
Field::constraint() const
{
	return m_constraint;
}

void
Field::setConstraint( const QSharedPointer< ConstraintBase > & c )
{
	m_constraint = c;
}

qint64
Field::lineNumber() const
{
	return m_lineNumber;
}

void
Field::setLineNumber( qint64 num )
{
	m_lineNumber = num;
}

qint64
Field::columnNumber() const
{
	return m_columnNumber;
}

void
Field::setColumnNumber( qint64 num )
{
	m_columnNumber = num;
}

bool
Field::isRequired() const
{
	return m_isRequired;
}

void
Field::setRequired( bool on )
{
	m_isRequired = on;
}

const QString &
Field::defaultValue() const
{
	return m_defaultValue;
}

void
Field::setDefaultValue( const QString & value )
{
	m_defaultValue = value;
}


//
// Class
//

Class::Class()
	:	m_lineNumber( -1 )
	,	m_columnNumber( -1 )
	,	m_index( 0 )
	,	m_parent( 0 )
{
}

Class::~Class()
{
}

Class::Class( const Class & other )
	:	m_name( other.name() )
	,	m_baseName( other.baseName() )
	,	m_baseValueType( other.baseValueType() )
	,	m_fields( other.fields() )
	,	m_lineNumber( other.lineNumber() )
	,	m_columnNumber( other.columnNumber() )
	,	m_index( other.index() )
{
}

Class &
Class::operator = ( const Class & other )
{
	if( this != &other )
	{
		m_name = other.name();
		m_baseName = other.baseName();
		m_baseValueType = other.baseValueType();
		m_fields = other.fields();
		m_lineNumber = other.lineNumber();
		m_columnNumber = other.columnNumber();
		m_index = other.index();
	}

	return *this;
}

bool
Class::operator == ( const QString & n ) const
{
	return ( name() == n );
}

const QString &
Class::name() const
{
	return m_name;
}

void
Class::setName( const QString & n )
{
	m_name = n;
}

const QString &
Class::baseName() const
{
	return m_baseName;
}

void
Class::setBaseName( const QString & n )
{
	m_baseName = n;
}

const QString &
Class::baseValueType() const
{
	return m_baseValueType;
}

void
Class::setBaseValueType( const QString & t )
{
	m_baseValueType = t;
}

const QList< Field > &
Class::fields() const
{
	return m_fields;
}

const Field *
Class::fieldByName( const QString & n ) const
{
	if( n.isEmpty() )
		return 0;

	QList< Field >::ConstIterator it = m_fields.constBegin();
	QList< Field >::ConstIterator last = m_fields.constEnd();

	for( ; it != last; ++it )
	{
		if( it->name() == n )
			return &(*it);
	}

	return 0;
}

Field *
Class::fieldByName( const QString & n )
{
	if( n.isEmpty() )
		return 0;

	QList< Field >::Iterator it = m_fields.begin();
	QList< Field >::Iterator last = m_fields.end();

	for( ; it != last; ++it )
	{
		if( it->name() == n )
			return &(*it);
	}

	return 0;
}

void
Class::addField( const Field & f )
{
	m_fields.append( f );
}

qint64
Class::lineNumber() const
{
	return m_lineNumber;
}

void
Class::setLineNumber( qint64 num )
{
	m_lineNumber = num;
}

qint64
Class::columnNumber() const
{
	return m_columnNumber;
}

void
Class::setColumnNumber( qint64 num )
{
	m_columnNumber = num;
}

quint64
Class::index() const
{
	return m_index;
}

void
Class::setIndex( quint64 i ) const
{
	m_index=  i;
}

const Namespace *
Class::parentNamespace() const
{
	return m_parent;
}

void
Class::setParentNamespace( const Namespace * p ) const
{
	m_parent = p;
}


//
// Namespace
//

Namespace::Namespace()
	:	m_lineNumber( -1 )
	,	m_columnNumber( -1 )
	,	m_parent( 0 )
{
}

Namespace::~Namespace()
{
}

Namespace::Namespace( const Namespace & other )
	:	m_name( other.name() )
	,	m_nestedNamespaces( other.allNested() )
	,	m_classes( other.classes() )
	,	m_lineNumber( other.lineNumber() )
	,	m_columnNumber( other.columnNumber() )
{
}

Namespace &
Namespace::operator = ( const Namespace & other )
{
	if( this != &other )
	{
		m_name = other.name();
		m_nestedNamespaces = other.allNested();
		m_classes = other.classes();
		m_lineNumber = other.lineNumber();
		m_columnNumber = other.columnNumber();
	}

	return *this;
}

const QString &
Namespace::name() const
{
	return m_name;
}

void
Namespace::setName( const QString & n )
{
	m_name = n;
}

const QList< Namespace > &
Namespace::allNested() const
{
	return m_nestedNamespaces;
}

QList< const Namespace* >
Namespace::nested( const QString & n ) const
{
	QList< const Namespace* > res;

	if( n.isEmpty() )
		return res;

	QList< Namespace >::ConstIterator it = m_nestedNamespaces.constBegin();
	QList< Namespace >::ConstIterator last = m_nestedNamespaces.constEnd();

	for( ; it != last; ++it )
	{
		if( it->name() == n )
			res.append( &(*it) );
	}

	return res;
}

QList< Namespace* >
Namespace::nested( const QString & n )
{
	QList< Namespace* > res;

	if( n.isEmpty() )
		return res;

	QList< Namespace >::Iterator it = m_nestedNamespaces.begin();
	QList< Namespace >::Iterator last = m_nestedNamespaces.end();

	for( ; it != last; ++it )
	{
		if( it->name() == n )
			res.append( &(*it) );
	}

	return res;
}

void
Namespace::addNamespace( const Namespace & n )
{
	m_nestedNamespaces.append( n );
}

const QList< Class > &
Namespace::classes() const
{
	return m_classes;
}

const Class *
Namespace::classByName( const QString & n ) const
{
	if( n.isEmpty() )
		return 0;

	QList< Class >::ConstIterator it = m_classes.constBegin();
	QList< Class >::ConstIterator last = m_classes.constEnd();

	for( ; it != last; ++it )
	{
		if( it->name() == n )
			return &(*it);
	}

	return 0;
}

Class *
Namespace::classByName( const QString & n )
{
	if( n.isEmpty() )
		return 0;

	QList< Class >::Iterator it = m_classes.begin();
	QList< Class >::Iterator last = m_classes.end();

	for( ; it != last; ++it )
	{
		if( it->name() == n )
			return &(*it);
	}

	return 0;
}

void
Namespace::addClass( const Class & c )
{
	m_classes.append( c );
}

qint64
Namespace::lineNumber() const
{
	return m_lineNumber;
}

void
Namespace::setLineNumber( qint64 num )
{
	m_lineNumber = num;
}

qint64
Namespace::columnNumber() const
{
	return m_columnNumber;
}

void
Namespace::setColumnNumber( qint64 num )
{
	m_columnNumber = num;
}

const Namespace *
Namespace::parentNamespace() const
{
	return m_parent;
}

void
Namespace::setParentNamespace( const Namespace * p ) const
{
	m_parent = p;
}


//
// Model
//

Model::Model()
{
}

Model::~Model()
{
}

Model::Model( const Model & other )
	:	m_root( other.rootNamespace() )
	,	m_globalIncludes( other.globalIncludes() )
	,	m_relativeIncludes( other.relativeIncludes() )
	,	m_includeGuard( other.includeGuard() )
{
}

Model &
Model::operator = ( const Model & other )
{
	if( this != &other )
	{
		m_root = other.rootNamespace();
		m_globalIncludes = other.globalIncludes();
		m_relativeIncludes = other.relativeIncludes();
		m_includeGuard = other.includeGuard();
	}

	return *this;
}

const Namespace &
Model::rootNamespace() const
{
	return m_root;
}

Namespace &
Model::rootNamespace()
{
	return m_root;
}

void
Model::setRootNamespace( const Namespace & n )
{
	m_root = n;
}

const QStringList &
Model::globalIncludes() const
{
	return m_globalIncludes;
}

void
Model::setGlobalIncludes( const QStringList & inc )
{
	m_globalIncludes = inc;
}

void
Model::addGlobalInclude( const QString & inc )
{
	m_globalIncludes.append( inc );
}

const QStringList &
Model::relativeIncludes() const
{
	return m_relativeIncludes;
}

void
Model::setRelativeIncludes( const QStringList & inc )
{
	m_relativeIncludes = inc;
}

void
Model::addRelativeInclude( const QString & inc )
{
	m_relativeIncludes.append( inc );
}

void extractAndBindAllClasses( const Namespace & root,
	QList< ConstClassPointer > & data )
{
	{
		QList< Class >::ConstIterator it = root.classes().constBegin();
		QList< Class >::ConstIterator last = root.classes().constEnd();

		for( ; it != last; ++it )
		{
			data.append( &(*it) );
			it->setParentNamespace( &root );
		}
	}

	{
		QList< Namespace >::ConstIterator it = root.allNested().constBegin();
		QList< Namespace >::ConstIterator last = root.allNested().constEnd();

		for( ; it != last; ++it )
		{
			it->setParentNamespace( &root );
			extractAndBindAllClasses( *it, data );
		}

	}
}

struct ConstClassPointerLess {
	bool operator () ( const ConstClassPointer & c1,
		const ConstClassPointer & c2 )
	{
		if( c1->lineNumber() < c2->lineNumber() )
			return true;
		else if( c1->lineNumber() == c2->lineNumber() )
			return ( c1->columnNumber() < c2->columnNumber() );
		else
			return false;
	}
};

void
Model::prepare()
{
	m_indexes.clear();

	QList< ConstClassPointer > sorted;

	extractAndBindAllClasses( m_root, sorted );

	std::sort( sorted.begin(), sorted.end(), ConstClassPointerLess() );

	quint64 index = 1;

	foreach( const ConstClassPointer & c, sorted )
	{
		c->setIndex( index );

		m_indexes.insert( index, c );

		++index;
	}
}

void
Model::check() const
{
	QStringList classes;

	ConstClassPointer c = 0;

	quint64 index = 0;

	const bool included = isIncluded();

	while( ( c = nextClass( index ) ) )
	{
		checkClass( *c, classes, included );
		++index;
	}
}

ConstClassPointer
Model::nextClass( quint64 index ) const
{
	index += 1;

	if( m_indexes.contains( index ) )
		return m_indexes[ index ];
	else
		return 0;
}

static inline QString fullName( const Class & c )
{
	QString name = c.name();

	const Namespace * nm = c.parentNamespace();

	while( nm )
	{
		if( !nm->name().isEmpty() )
		{
			name.prepend( c_namespaceSeparator );
			name.prepend( nm->name() );
		}

		nm = nm->parentNamespace();
	}

	return name;
}

static inline bool
checkIsClassDefined( const QString & classToCheck,
	const QString & thisClassName,
	const QStringList & prevDefinedClasses )
{
	const QStringList toCheck = classToCheck.split( c_namespaceSeparator,
		QString::SkipEmptyParts );

	const QStringList thisClass = thisClassName.split( c_namespaceSeparator,
		QString::SkipEmptyParts );

	const int min = qMin( toCheck.size(), thisClass.size() );

	foreach( const QString & c, prevDefinedClasses )
	{
		QStringList toCheckTmp = toCheck;
		QStringList thisClassTmp = thisClass;
		QStringList classTmp = c.split( c_namespaceSeparator,
			QString::SkipEmptyParts );

		if( toCheckTmp == classTmp )
		{
			const int minLength = qMin( min, classTmp.size() );

			for( int i = 1; i < minLength; ++i )
			{
				if( toCheckTmp.first() == thisClassTmp.first() &&
					toCheckTmp.first() == classTmp.first() )
				{
					toCheckTmp.removeFirst();
					thisClassTmp.removeFirst();
					classTmp.removeFirst();
				}
				else
					break;
			}

			if( toCheckTmp.size() == 1 || thisClassTmp.size() == 1 )
				return true;
		}
	}

	return false;
}

void
Model::checkClass( const Class & c,
	QStringList & prevDefinedClasses, bool included ) const
{
	const QString className = fullName( c );

	if( prevDefinedClasses.contains( className ) )
		throw QtConfFile::Exception( QString( "Redefinition of class \"%1\". "
			"Line %2, column %3." )
				.arg( className )
				.arg( QString::number( c.lineNumber() ) )
				.arg( QString::number( c.columnNumber() ) ) );

	const QString & base = c.baseName();

	if( !included )
	{
		if( base != c_scalarTagName && base != c_noValueTagName &&
			base != c_scalarVectorTagName && base != c_vectorOfTagsTagName )
		{
			if( !checkIsClassDefined( base, className, prevDefinedClasses ) )
			{
				throw QtConfFile::Exception( QString( "Base class \"%1\" "
					"of class \"%2\" wasn't defined. Line %3, column %4." )
						.arg( base )
						.arg( className )
						.arg( QString::number( c.lineNumber() ) )
						.arg( QString::number( c.columnNumber() ) ) );
			}
		}
	}

	prevDefinedClasses.append( className );

	QStringList fields;

	foreach( const Field & f, c.fields() )
	{
		if( fields.contains( f.name() ) )
			throw QtConfFile::Exception( QString( "Field \"%1\" "
				"already defined in class \"%2\". Line %3, column %4." )
					.arg( f.name() )
					.arg( className )
					.arg( QString::number( f.lineNumber() ) )
					.arg( QString::number( f.columnNumber() ) ) );
		else
			fields.append( f.name() );

		if( !included )
		{
			if( f.type() == Field::CustomTagFieldType )
			{
				if( !checkIsClassDefined( f.valueType(), className,
					prevDefinedClasses ) )
				{
					throw QtConfFile::Exception( QString( "Value type \"%1\" of "
						"member \"%2\" of class \"%3\" "
						"wasn't defined. Line %4, column %5." )
							.arg( f.valueType() )
							.arg( f.name() )
							.arg( className )
							.arg( QString::number( c.lineNumber() ) )
							.arg( QString::number( c.columnNumber() ) ) );
				}
			}
		}
	}
}

bool
Model::isIncluded() const
{
	return ( !m_globalIncludes.isEmpty() || !m_relativeIncludes.isEmpty() );
}

const QString &
Model::includeGuard() const
{
	return m_includeGuard;
}

void
Model::setIncludeGuard( const QString & guard )
{
	m_includeGuard = guard;
}


//
// TagMinMaxConstraint
//

TagMinMaxConstraint::TagMinMaxConstraint( QtConfFile::Tag & owner )
	:	QtConfFile::TagNoValue( owner, c_minMaxConstraintTagName, false )
	,	m_min( *this, c_minTagName, true )
	,	m_max( *this, c_maxTagName, true )
{
}

TagMinMaxConstraint::~TagMinMaxConstraint()
{
}

QSharedPointer< MinMaxConstraint >
TagMinMaxConstraint::cfg() const
{
	return QSharedPointer< MinMaxConstraint > ( new MinMaxConstraint(
		m_min.value(), m_max.value() ) );
}


//
// TagOneOfConstraint
//

TagOneOfConstraint::TagOneOfConstraint( QtConfFile::Tag & owner )
	:	QtConfFile::TagScalarVector< QString > ( owner,
			c_oneOfConstraintTagName, false )
{
}

TagOneOfConstraint::~TagOneOfConstraint()
{
}

QSharedPointer< OneOfConstraint >
TagOneOfConstraint::cfg() const
{
	return QSharedPointer< OneOfConstraint > ( new OneOfConstraint(
		values().toList() ) );
}


//
// TagField
//

TagField::TagField( const QString & name, bool isMandatory )
	:	QtConfFile::TagNoValue( name, isMandatory )
	,	m_name( *this, c_fieldNameTagName, true )
	,	m_valueType( *this, c_valueTypeTagName, false )
	,	m_minMaxConstraint( *this )
	,	m_oneOfConstraint( *this )
	,	m_isRequired( *this, c_requiredTagName, false )
	,	m_defaultValue( *this, c_defaultValueTagName, false )
{
}

TagField::~TagField()
{
}

static inline Field::FieldType fieldTypeFromString( const QString & type )
{
	if( type == c_scalarTagName )
		return Field::ScalarFieldType;
	else if( type == c_noValueTagName )
		return Field::NoValueFieldType;
	else if( type == c_scalarVectorTagName )
		return Field::ScalarVectorFieldType;
	else if( type == c_vectorOfTagsTagName )
		return Field::VectorOfTagsFieldType;
	else if( type == c_customTagName )
		return Field::CustomTagFieldType;
	else
		return Field::UnknownFieldType;
}

static inline void throwConstraintRedefinition( const QString & className,
	const QString & fieldName, qint64 lineNumber, qint64 columnNumber )
{
	throw QtConfFile::Exception( QString( "Redefinition of "
		"constraint in class \"%1\", field \"%2\". "
		"Line %3, column %4." )
			.arg( className )
			.arg( fieldName )
			.arg( QString::number( lineNumber ) )
			.arg( QString::number( columnNumber ) ) );
}

Field
TagField::cfg() const
{
	Field f;
	f.setType( fieldTypeFromString( name() ) );
	f.setName( m_name.value() );
	f.setLineNumber( lineNumber() );
	f.setColumnNumber( columnNumber() );

	if( m_valueType.isDefined() )
		f.setValueType( m_valueType.value() );

	if( m_isRequired.isDefined() )
		f.setRequired();

	if( m_defaultValue.isDefined() )
		f.setDefaultValue( m_defaultValue.value() );
	else if( f.type() == Field::NoValueFieldType )
		f.setDefaultValue( QLatin1String( "false" ) );

	if( m_minMaxConstraint.isDefined() && m_oneOfConstraint.isDefined() )
	{
		const TagClass * c = static_cast< const TagClass* > ( parent() );

		if( m_minMaxConstraint.lineNumber() == m_oneOfConstraint.lineNumber() )
		{
			if( m_minMaxConstraint.columnNumber() > m_oneOfConstraint.columnNumber() )
				throwConstraintRedefinition( c->value(), m_name.value(),
					m_minMaxConstraint.lineNumber(),
					m_minMaxConstraint.columnNumber() );
			else
				throwConstraintRedefinition( c->value(), m_name.value(),
					m_oneOfConstraint.lineNumber(),
					m_oneOfConstraint.columnNumber() );
		}
		else if( m_minMaxConstraint.lineNumber() > m_oneOfConstraint.lineNumber() )
			throwConstraintRedefinition( c->value(), m_name.value(),
				m_minMaxConstraint.lineNumber(),
				m_minMaxConstraint.columnNumber() );
		else
			throwConstraintRedefinition( c->value(), m_name.value(),
				m_oneOfConstraint.lineNumber(),
				m_oneOfConstraint.columnNumber() );
	}

	if( m_minMaxConstraint.isDefined() )
		f.setConstraint( m_minMaxConstraint.cfg() );
	else if( m_oneOfConstraint.isDefined() )
		f.setConstraint( m_oneOfConstraint.cfg() );

	return f;
}

void
TagField::onFinish( const ParserInfo & info )
{
	switch( fieldTypeFromString( name() ) )
	{
		case Field::ScalarFieldType :
		case Field::ScalarVectorFieldType :
		case Field::VectorOfTagsFieldType :
		{
			if( !m_valueType.isDefined() )
				throw QtConfFile::Exception( QString( "Undefined required "
					"tag \"%1\" in tag \"%2\". Line %3, column %4." )
						.arg( c_valueTypeTagName )
						.arg( name() )
						.arg( QString::number( info.lineNumber() ) )
						.arg( QString::number( info.columnNumber() ) ) );
		}
			break;

		default :
			break;
	}

	QtConfFile::TagNoValue::onFinish( info );
}


//
// TagBaseClass
//

TagBaseClass::TagBaseClass( QtConfFile::Tag & owner, const QString & name,
	bool isMandatory )
	:	QtConfFile::TagScalar< QString > ( owner, name, isMandatory )
	,	m_valueType( *this, c_valueTypeTagName, false )
{
}

TagBaseClass::~TagBaseClass()
{
}

QString
TagBaseClass::valueType() const
{
	return m_valueType.value();
}

void
TagBaseClass::onFinish( const ParserInfo & info )
{
	switch( fieldTypeFromString( value() ) )
	{
		case Field::ScalarFieldType :
		case Field::ScalarVectorFieldType :
		case Field::VectorOfTagsFieldType :
		{
			if( !m_valueType.isDefined() )
				throw QtConfFile::Exception( QString( "Undefined required "
					"tag \"%1\" in tag \"%2\". Line %3, column %4." )
						.arg( c_valueTypeTagName )
						.arg( value() )
						.arg( QString::number( info.lineNumber() ) )
						.arg( QString::number( info.columnNumber() ) ) );
		}
			break;

		default :
			break;
	}

	QtConfFile::TagScalar< QString >::onFinish( info );
}


//
// TagClass
//

TagClass::TagClass( const QString & name, bool isMandatory )
	:	QtConfFile::TagScalar< QString > ( name, isMandatory )
	,	m_baseClassName( *this, c_baseClassTagName, true )
	,	m_scalarTags( *this, c_scalarTagName, false )
	,	m_noValueTags( *this, c_noValueTagName, false )
	,	m_scalarVectorTags( *this, c_scalarVectorTagName, false )
	,	m_vectorOfTagsTags( *this, c_vectorOfTagsTagName, false )
	,	m_customTags( *this, c_customTagName, false )
{
}

TagClass::~TagClass()
{
}

Class
TagClass::cfg() const
{
	Class c;
	c.setName( value() );
	c.setBaseName( m_baseClassName.value() );
	c.setBaseValueType( m_baseClassName.valueType() );
	c.setLineNumber( lineNumber() );
	c.setColumnNumber( columnNumber() );

	if( m_scalarTags.isDefined() )
	{
		for( int i = 0; i < m_scalarTags.size(); ++i )
			c.addField( m_scalarTags.at( i ).cfg() );
	}

	if( m_noValueTags.isDefined() )
	{
		for( int i = 0; i < m_noValueTags.size(); ++i )
			c.addField( m_noValueTags.at( i ).cfg() );
	}

	if( m_scalarVectorTags.isDefined() )
	{
		for( int i = 0; i < m_scalarVectorTags.size(); ++i )
			c.addField( m_scalarVectorTags.at( i ).cfg() );
	}

	if( m_vectorOfTagsTags.isDefined() )
	{
		for( int i = 0; i < m_vectorOfTagsTags.size(); ++i )
			c.addField( m_vectorOfTagsTags.at( i ).cfg() );
	}

	if( m_customTags.isDefined() )
	{
		for( int i = 0; i < m_customTags.size(); ++i )
			c.addField( m_customTags.at( i ).cfg() );
	}

	return c;
}


//
// TagNamespace
//

TagNamespace::TagNamespace( const QString & name, bool isMandatory )
	:	QtConfFile::TagScalar< QString > ( name, isMandatory )
	,	m_nestedNamespaces( *this, c_namespaceTagName, false )
	,	m_classes( *this, c_classTagName, false )
{
}

TagNamespace::~TagNamespace()
{
}

Namespace
TagNamespace::cfg() const
{
	Namespace n;
	n.setName( value() );
	n.setLineNumber( lineNumber() );
	n.setColumnNumber( columnNumber() );

	if( m_nestedNamespaces.isDefined() )
	{
		for( int i = 0; i < m_nestedNamespaces.size(); ++i )
			n.addNamespace( m_nestedNamespaces.at( i ).cfg() );
	}

	if( m_classes.isDefined() )
	{
		for( int i = 0; i < m_classes.size(); ++i )
			n.addClass( m_classes.at( i ).cfg() );
	}

	return n;
}


//
// TagModel
//

TagModel::TagModel()
	:	QtConfFile::TagScalar< QString > ( c_mainCfgTagName, true )
	,	m_rootNamespace( *this, c_namespaceTagName, false )
	,	m_rootClasses( *this, c_classTagName, false )
	,	m_globalIncludes( *this, c_globalIncludeTagName, false )
	,	m_relativeIncludes( *this, c_relativeIncludeTagName, false )
{
}

TagModel::~TagModel()
{
}

Model
TagModel::cfg() const
{
	Model m;
	m.setIncludeGuard( value() );

	if( m_rootNamespace.isDefined() )
	{
		for( int i = 0; i < m_rootNamespace.size(); ++i )
			m.rootNamespace().addNamespace( m_rootNamespace.at( i ).cfg() );
	}

	if( m_rootClasses.isDefined() )
	{
		for( int i = 0; i < m_rootClasses.size(); ++i )
			m.rootNamespace().addClass( m_rootClasses.at( i ).cfg() );
	}

	if( m_globalIncludes.isDefined() )
	{
		for( int i = 0; i < m_globalIncludes.size(); ++i )
			m.addGlobalInclude( m_globalIncludes.at( i ).value() );
	}

	if( m_relativeIncludes.isDefined() )
	{
		for( int i = 0; i < m_relativeIncludes.size(); ++i )
			m.addRelativeInclude( m_relativeIncludes.at( i ).value() );
	}

	return m;
}

} /* namespace Cfg */

} /* namespace Generator */

} /* namespace QtConfFile */
