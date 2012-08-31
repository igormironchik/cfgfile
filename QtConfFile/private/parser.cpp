
/*!
    \file

    \author Igor Mironchik (igor.mironchik at gmail dot com).

    Copyright (c) 2012 Igor Mironchik

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

// QtConfFile include.
#include <QtConfFile/private/Parser>
#include <QtConfFile/private/Lex>
#include <QtConfFile/private/InputStream>
#include <QtConfFile/private/Tag>
#include <QtConfFile/Exceptions>


namespace QtConfFile {

//
// Parser::ParserPrivate
//

class Parser::ParserPrivate {
public:
    ParserPrivate( const Tag & tag, InputStream & stream )
        :   m_lex( stream )
        ,   m_tag( tag )
    {
    }

    LexicalAnalyzer m_lex;
    const Tag & m_tag;
}; // class Parser::ParserPrivate


//
// Parser
//

Parser::Parser( const Tag & tag, InputStream & stream )
    :   d( new ParserPrivate( tag, stream ) )
{
}

Parser::~Parser()
{
}

void
Parser::parse()
{
}

} /* namespace QtConfFile */
