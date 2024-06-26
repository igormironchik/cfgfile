[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

Library for reading and writing configuration files (`cfgfile`).

* [Compilling](#compilling)
* [Q/A](#qa)
  * [How can I add `cfgfile` to my project?](#how-can-i-add-cfgfile-to-my-project)
* [About](#about)
* [Example](#example)
* [Generator](#generator)
  * [Example](#example)
* [Why to use `cfgfile`?](#why-to-use-cfgfile)

# Compilling

This is header-only library. But if you want to build examples and tests just
use `CMake`.

To build with `Qt` support define `CFGFILE_QT_SUPPORT`. To build with `XML` support
define `CFGFILE_QT_SUPPORT` and `CFGFILE_XML_SUPPORT`. `XML` supported only with `Qt`.

To disable `STL` strings define `CFGFILE_DISABLE_STL`. It can be useful in collaboration
with defined `CFGFILE_QT_SUPPORT` on `Android`.

# Q/A

How can I add `cfgfile` to my project?
---

 * The simplest way is just copy `cfgfile` directory with headers to any location in
your project. With `CMake` you can clone entire `cfgfile` project somewhere in your
project and just do `add_subdirectory()`, if you will do so you have to
add include directory path to your project with
`include_directories( ${cfgfile_INCLUDE_DIRECTORIES} )`.

# About

Configuration file format is a set of tags, which are surrounded by curly
brackets, with values. Tags can be without values, can have one, and can have
several values. A sample configuration file is shown below:

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

String tag's values can be framed in quotation marks if it contains white space
or special characters. The special characters are: `\n`, `\r`, `\t`, `\"`, `\\`, is the
line breaks, tabs, quotes and backslash.

Parsing of the configuration file is based on a specialized class that inherits
from `cfgfile::tag_t` or any derived class. For each tag in this case, define a
class member that is the object of a class derived from `cfgfile::tag_t`.
There are finished classes in cfgfile for:

 * `tag_scalar_t< T, Trait >` - tag with a single value,
 * `tag_scalar_vector_t< T, Trait >` - tag with a set of values,
 * `tag_no_value_t< Trait >` - tag with no value,
 * `tag_vector_of_tags_t< T, Trait >` - tag with multiple entries of subordinate tag.

Each tag can have nested tags. Nesting is not limited, except your needs for
this. You must inherit from the appropriate class tag and provide the required
number of members representing the nested tags.

# Example

Let's say we need ability to read and write configuration file of the following
format:

```
{configuration
  {ourCoolValue <std::string>}
}
```

I.e. we want to have parent tag `{configuration}` and child tag `{ourCoolValue}`
that will have one value of type `std::string`. It's very simple to define such
configuration with `cfgfile`. Let's see:

```cpp
#include <cfgfile/all.hpp>

//
// Configuration
//

//! This is data class that will store our configuration in the application.
class Configuration {
public:
  Configuration()
  {
  }

  explicit Configuration( const std::string & value )
    :  m_ourCoolValue( value )
  {
  }

  ~Configuration()
  {
  }

  //! \return Our cool value.
  const std::string & ourCoolValue() const
  {
    return m_ourCoolValue;
  }

private:
  //! Our cool value.
  std::string m_ourCoolValue;
}; // class Configuration


//
// TagConfiguration
//

//! This is cfgfile tag, uses to read and write configuration.
class TagConfiguration
  :  public cfgfile::tag_no_value_t<>
{
public:
  TagConfiguration()
    :  cfgfile::tag_no_value_t<>( "configuration", true )
    ,  m_ourCoolValue( *this, "ourCoolValue", true )
  {
  }

  explicit TagConfiguration( const Configuration & cfg )
    :  cfgfile::tag_no_value_t<>( "configuration", true )
    ,  m_ourCoolValue( *this, "ourCoolValue", true )
  {
    m_ourCoolValue.set_value( cfg.ourCoolValue() );

    set_defined();
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
  cfgfile::tag_scalar_t< std::string > m_ourCoolValue;
}; // class TagConfiguration
```

We need some struct/class to store data of configuration in the application,
so we defined class `Configuration`. But this is only data class. To
work with files (i.e. read and write configuration from/to file) we need to
define class derived from `cfgfile::tag_t`. As our parent tag `{configuration}`
doesn't have any values we derived our tag class from `cfgfile::tag_no_value_t`.
Our parent tag should have child tag with one value of type `std::string`,
so we can use `cfgfile::tag_scalar_t< std::string >` for it.

Child tags must receive parent tag as argument in constructor. Second argument
is the name of the tag, and third is the flag required our child tag or not.

Second constructor uses to save configuration, and first - for reading.

We defined method for constructing `Configuration` from parsed tag.
As all tags are required we did not do any checks. But if you have not required
tag in the configuration then you can use `cfgfile::tag_t::is_defined()` method to
check if tag was defined.

When we have data class and tag class, so it's very simple to read and write
configuration. Let's see:

```cpp
Configuration cfg;

std::ifstream stream( "fileName.cfg" );

try {
  TagConfiguration readTag;

  cfgfile::read_cfgfile( readTag, stream, "fileName.cfg" );

  stream.close();

  cfg = readTag.cfg();
}
catch( const cfgfile::exception_t<> & x )
{
  stream.close();

  std::cout << x.desc() << std::endl;
}
```

And:

```cpp
Configuration cfg( "value" );

std::ofstream stream( "fileName.cfg" );

try {
  TagConfiguration writeTag( cfg );

  cfgfile::write_cfgfile( writeTag, stream );

  stream.close();
}
catch( const cfgfile::exception_t<> & x )
{
  stream.close();

  std::cout << x.desc(); << std::endl
}
```

# Generator

To simplify development with `cfgfile` was implemented `cfgfile` generator.

`cfgfile` generator this is generator of C++ header file from
declarative description of the configuration file. By configuration file
assumes configuration file in `cfgfile` format. In generated header will
be declared all necessary classes of data and configuration tags.

For generation generator uses input configuration file in `cfgfile` format
with following format:

Let's say you need data class with one `std::string` field then you should write
following configuration file, for example:

```
{forGeneration ifndefMacro
  {namespace NamespaceName
    {class NameOfTheClass
      {base tagNoValue}

      {tagScalar
        {valueType std::string}
        {name fieldWithString}
      }
    }
  }
}
```

Each configuration file for generator must has one `{forGeneration}` tag and all
nested tags must be defined in this tag. `{forGeneration}` tag require string for
`#ifndef` directive (include guard). There you can use `{namespace}` tag for
generation data classes and tags in the specified C++ namespace. Namespaces can
be nested. To declare data class and tag you should use `{class}` tag.
`{class}` tag can has `{base}` tag to define base class of the configuration tag.
`{base}` tag can has following values: `tagNoValue`, `tagScalar`, `tagScalarVector`.
`tagNoValue` is not mandatory in this case. To define field in data class you can
use following tags: `{tagScalar}` for scalar tag, `{tagNoValue}` for no-value tag,
`{tagScalarVector}` for vector of scalar values, `{tag}` for already defined tag,
`{tagVectorOfTags}` for vector of already defined tags.

`{tagScalar}`, `{tagScalarVector}` require C++ type defined in `{valueType}` tag.
In this situation C++ type should be defined as string, that mean that, for
example, `unsigned int` C++ type should be declared as `"unsigned int"`.

`{tag}` and `{tagVectorOfTags}` require class name with namespace if it is exist.
Class name should be defined in `{valueType}` tag.
Namespace and class name should be separated with `::`

All of field tags must have `{name}` tag with name of the field as value. If
`{base}` defined with `tagScalar` or `tagScalarVector` then `{valueType}` and `{name}`
must be defined. `{base tagNoValue}` is not necessary, if `{base}` is not defined
then `tagNoValue` will be used as base class.

Fields can have `{required}` tag to mark field as required.

All of field tags can have `{minMaxConstraint}` or `{oneOfConstraint}` to define
constraint for the field's value. `{minMaxConstraint}` require two tags
`{min <string>}` and `{max <string>}`, and `{oneOfConstraint}` require list of values.
For example, `{minMaxConstraint {min 0} {max 100}}`,
`{oneOfConstraint one two three}`.

`{base}` and fields can have `{defaultValue}` tag to specify default value.

In the above example will be generated similar to the following header file.

```cpp
namespace NamespaceName {

//
// NameOfTheClass
//

class NameOfTheClass {
public:
  c_tors();
  ~d_tor();

  //! \return fieldWithQString value.
  const std::string & fieldWithString() const;
  //! Set fieldWithString value.
  void set_fieldWithString( const std::string & value );
}; // class NameOfTheClass


//
// tag_NameOfTheClass
//

template< typename Trait >
class tag_NameOfTheClass
  :  public cfgfile::tag_no_value_t< Trait >
{
public:
  c_tors();
  ~d_tor();

  //! \return Configuration.
  NameOfTheClass get_cfg() const;
}; // class tag_NameOfTheClass

} // namespace NamespaceName
```

Additionally you can use custom data structures and tags in the generator.
For this you can use `{globalInclude <string>}` and `{relativeInclude <string>}`
tags. When using such includes in C++ code will be added corresponding
include directives. And checking of classes' and namespaces' names will be
turned off. And one more restriction is that that class's name for tag
must be equal to `tag_ + Name`, i.e. if data structure names `Data` then class for
tag must be named `tag_Data` and be placed in the same namespace as `Data` class/
structure.

## Example

Look at example/generator and tests/auto/Generator

# Why to use `cfgfile`?

... whereas there are a lot of another solutions with less coding
for example ...

Very good question. As you can see `cfgfile` uses non-standard format of file, I use
curly braces for each tag in the file. I have `XML` support with `Qt`, but it's non-standard
too. So why people should have a look at `cfgfile`? Look for example at `json_dto` by
`Stiffstream`. It's `JSON`, it's standard, so little coding is necessary, just one method
in struct and serialization/deserialization is done, just a few line of code...
I position `cfgfile` as for configurations that can be read by human, that easy to
navigate and look at the text files. What about very verbose coding necessary with
`cfgfile` I can say that I implemented generator for `cfgfile`, so coding comes
down to simple config file for generator.

Look at the example of configuration file for `cfgfile` generator in a real project.
[This file](https://github.com/igormironchik/prototyper/blob/master/src/Core/project_cfg.qtconf)
describes very complicated configuration of the project for some kind of vector graphics
editor.

[Look at this file](https://github.com/igormironchik/prototyper/blob/master/example/gif-editor.prototyper)
that include a lot of images, look how it's simple to see the block of data of the image.

I want to say that my main idea was to make configuration files easy to read by
human.
