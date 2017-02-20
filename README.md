
Library for reading and writing configuration files with Qt (QtConfFile).

# LICENSE

Copyright (c) 2012-2016 Igor Mironchik

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


# COMPILING

 * Juct run qmake and then make, as usually.

# About

Configuration file format is a set of tags, which are surrounded by curly brackets, with values.
Tags can be without values, can have one, and can have several values. A sample configuration
file is shown below:

```          
{cfg
  {stringValue string}
  {listOfStringValues str1 str2 str3}
  {intValue 100}
              
  {vecOfTags
    {stringValue string1}
    {intValue 100}
  }
              
  {vecOfTags
    {stringValue string2}
    {intValue 200}
  }
}
```
          
String tag's values can be framed in quotation marks if it contains white space or special characters.
The special characters are: \n, \r, \t, \", \\, is the line breaks, tabs, quotes and backslash.
          
Parsing of the configuration file is based on a specialized class that inherits from QtConfFile::Tag
or any derived class. For each tag in this case, define a class member that is the object of a class
derived from QtConfFile::Tag. There are finished classes in QtConfFile for:
          
 * TagScalar< T > — tag with a single value,
 * TagScalarVector< T > — tag with a set of values,
 * TagNoValue — tag with no value,
 * TagVectorOfTags< T > — tag with multiple entry subordinate tag.
          
Each tag can have nested tags. Nesting is not limited, except your needs for this. You must
inherit from the appropriate class tag and provide the required number of members representing
the nested tags.
          
# Example
            
Let's say we need ability to read and write configuration file of the following format:

```            
{configuration
  {ourCoolValue <QString>}
}
```

I.e. we want to have parent tag {configuration} and child tag {ourCoolValue} that will
have one value of type QString. It's very simple to define such configuration with
QtConfFile. Let's see:
            
```
#include <QtConfFile/TagNoValue>
#include <QtConfFile/TagScalar>
#include <QtConfFile/Utils>

//
// Configuration
//

//! This is data class that will store our configuration in the application.
class Configuration {
public:
  Configuration()
  {
  }
  
  explicit Configuration( const QString & value )
    :  m_ourCoolValue( value )
  {
  }
  
  explicit Configuration( const Configuration &amp; other )
    :  m_ourCoolValue( other.ourCoolValue() )
  {
  }
  
  Configuration &amp; operator = ( const Configuration &amp; other )
  {
    if( this != &other )
    {
      m_ourCoolValue = other.ourCoolValue();
    }
    
    return *this;
  }
  
  ~Configuration()
  {
  }
  
  //! \return Our cool value.
  const QString &amp; ourCoolValue() const
  {
    return m_ourCoolValue;
  }
  
private:
  //! Our cool value.
  QString m_ourCoolValue;
}; // class Configuration


//
// TagConfiguration
//

//! This is QtConfFile tag, uses to read and write configuration.
class TagConfiguration
  :  public QtConfFile::TagNoValue
{
public:
  TagConfiguration()
    :  QtConfFile::TagNoValue( QLatin1String( "configuration" ), true )
    ,  m_ourCoolValue( *this, QLatin1String( "ourCoolValue" ), true )
  {
  }
  
  explicit TagConfiguration( const Configuration &amp; cfg )
    :  QtConfFile::TagNoValue( QLatin1String( "configuration" ), true )
    ,  m_ourCoolValue( *this, QLatin1String( "ourCoolValue" ), true )
  {
    m_ourCoolValue.setValue( cfg.ourCoolValue() );
    
    setDefined();
  }
  
  ~TagConfiguration()
  {
  }
  
  //! \return Configuration read from file.
  Configuration cfg() const
  {
    return Configuration( m_ourCoolValue.value() );
  }
  
private:
  //! Our cool value.
  QtConfFile::TagScalar< QString > m_ourCoolValue;
}; // class TagConfiguration
```

As our configuration consist one parent tag {configuration} without value and
one child tag {ourCoolValue} with one value of type QString, so we included
TagNoValue and TagScalar headers. Utils needed to read and write files.
            
We need some struct/class to store data of configuration in the application,
so we defined class Configuration on line 10. But this is only data class. To
work with files (i.e. read and write configuration from/to file) we need to
define class derived from QtConfFile::Tag. As our parent tag {configuration}
doesn't have any values we derived our tag class from QtConfFile::TagNoValue, look
at line 58. Our parent tag should have child tag with one value of type QString,
so we can use QtConfFile::TagScalar for it, look at line 88.</p>
            
Child tags must receive parent tag as argument in constructor, look at lines
63 and 69. Second argument is the name of the tag, and third is the flag recuired our
child tag or not.
            
Second constructor uses to save configuration, and first - for reading.
            
On line 81 we defined method for constructing Configuration from parsed tag.
As all tags are required we did not do any checks. But if you have not required
tag in the configuration then you can use QtConfFile::Tag::isDefined() method to
check if tag was defined.
            
When we have data class and tag class it's very simple to read and write configuration.
Let's see:
            
```
Configuration cfg;
            
try {
  TagConfiguration readTag;
  
  QtConfFile::readQtConfFile( readTag, QLatin1String( "fileName.cfg" ),
    QTextCodec::codecForName( "UTF-8" ) );
  
  cfg = readTag.cfg();
}
catch( const QtConfFile::Exception &amp; x )
{
  qDebug() << x.whatAsQString();
}
```

And:
            
```
Configuration cfg( QLatin1String( "value" ) );
            
try {
  TagConfiguration writeTag( cfg );
  
  QtConfFile::writeQtConfFile( writeTag, QLatin1String( "fileName.cfg" ),
    QTextCodec::codecForName( "UTF-8" ) );
}
catch( const QtConfFile::Exception &amp; x )
{
  qDebug() << x.whatAsQString();
}
```

# Generator
            
To simplify development with QtConfFile was implemented QtConfFile generator.
            
QtConfFile generator this is generator of C++ header file from
declarative description of the configuration file. By configuration file
assumes configuration file in QtConfFile format. In generated header will
be declared all necessary classes of data and configuration tags.

For generation generator uses input configuration file in QtConfFile format
with following format:

Let's say you need data class with one QString field then you should write
following configuration file, for example:

```
{forGeneration ifndefMacro
  {namespace NamespaceName
    {class NameOfTheClass
      {base tagNoValue}
      
      {tagScalar
        {valueType QString}
        {name fieldWithQString}
      }
    }
  }
}
```

Each configuration file for generator must has one {forGeneration} tag and all
nested tags must be defined in this tag. {forGeneration} tag require string for
`#ifndef` directive (include guard). There you can use {namespace} tag for
generation data classes and tags in the specified C++ namespace. Namespaces can
be nested. To declare data class and tag you should use {class} tag.
{class} tag can has {base} tag to define base class of the configuration tag.
{base} tag can has following values: tagNoValue, tagScalar, tagScalarVector.
tagNoValue is not mandatory in this case. To define field in data class you can
use following tags: {tagScalar} for scalar tag, {tagNoValue} for no-value tag,
{tagScalarVector} for vector of scalar values, {tag} for already defined tag,
{tagVectorOfTags} for vector of already defined tags.

{tagScalar}, {tagScalarVector} require C++ type defined in {valueType} tag.
In this situation C++ type should be defined as string, that mean that, for
example, unsigned int C++ type should be declared as "unsigned int".

{tag} and {tagVectorOfTags} require class name with namespace if it is exist.
Class name should be defined in {valueType} tag.
Namespace and class name should be separated with ::

All of field tags must have {name} tag with name of the field as value. If
{base} defined with tagScalar or tagScalarVector then {valueType} and {name}
must be defined. {base tagNoValue} is not necessary, if {base} is not defined
then tagNoValue will be used as base class.

Fields can have {required} tag to mark field as required.

All of field tags can have {minMaxConstraint} or {oneOfConstraint} to define
constraint for the field's value. {minMaxConstraint} require two tags
{min <string>} and {max <string>}, and {oneOfConstraint} require list of values.
For example, {minMaxConstraint {min 0} {max 100}},
{oneOfConstraint one two three}.

In the above example will be generated similar to the following header file.

```
namespace NamespaceName {

//
// NameOfTheClass
//
  
class NameOfTheClass {
public:
  c_tors();
  ~d_tor();
    
  //! \return fieldWithQString value.
  const QString &amp; fieldWithQString() const;
  //! Set fieldWithQString value.
  void setFieldWithQString( const QString &amp; value );
}; // class NameOfTheClass
  
  
//
// TagNameOfTheClass
//
  
class TagNameOfTheClass
  :  public QtConfFile::TagNoValue
{
public:
  c_tors();
  ~d_tor();
    
  //! \return Configuration.
  NameOfTheClass getCfg() const;
}; // class TagNameOfTheClass
  
} // namespace NamespaceName
```

Additionally you can use custom data structures and tags in the generator.
For this you can use {globalInclude <string>} and {relativeInclude <string>}
tags. When using such includes in C++ code will be added corresponding
include directives. And checking of classes' and namespaces' names will be
turned off. And one more restriction is that that class's name for tag
must be equal to Tag + Name, i.e. if data structure names Data then class for
tag must be named TagData and be placed in the same namespace as Data class/
structure.
            
# Integration of Generator with QMake
            
To integrate QtConfFile generator with qmake you need to add lines to the
qmake's pro file similar to the next:
            
```
TO_GENERATE = example.qtconffile

QMAKE_EXTRA_COMPILERS += generate_cfg
generate_cfg.name = CONF_GEN
generate_cfg.input = TO_GENERATE
generate_cfg.output = ${QMAKE_FILE_BASE}.hpp
generate_cfg.CONFIG = no_link
generate_cfg.variable_out = HEADERS

generate_cfg.commands = $$shell_path( $$absolute_path( path/to/qtconffile.generator ) ) \
-i ${QMAKE_FILE_IN} -o $${OUT_PWD}/${QMAKE_FILE_BASE}.hpp

PRE_TARGETDEPS += compiler_generate_cfg_make_all
```
