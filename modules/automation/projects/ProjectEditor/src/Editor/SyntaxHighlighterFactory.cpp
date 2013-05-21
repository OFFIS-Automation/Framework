// OFFIS Automation Framework
// Copyright (C) 2013 OFFIS e.V.
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "SyntaxHighlighterFactory.h"

#include "SyntaxHighlighter/PyHighlighter.h"
#include "SyntaxHighlighter/XmlHighlighter.h"
#include "SyntaxHighlighter/TxtHighlighter.h"

SyntaxHighlighterFactory::SyntaxHighlighterFactory()
{
}

void SyntaxHighlighterFactory::create(const QString &filename, QTextDocument *document)
{
    if (filename.endsWith(".py"))
        new PyHighlighter(document);
    else if (filename.endsWith(".xml") || filename.endsWith(".ogr"))
        new XMLHighlighter(document);
    else if(filename.endsWith(".pro") || filename.endsWith(".ini")  || filename.endsWith(".oap"))
        new TxtHighlighter(document);
}
