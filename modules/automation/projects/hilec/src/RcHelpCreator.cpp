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

#include "RcHelpCreator.h"

#include "HilecSingleton.h"

#include <core/RcUnitHelp.h>


RcHelpCreator::RcHelpCreator(const QString &name)
{
    mStream.setString(&mBuffer, QIODevice::WriteOnly);

    RcUnitHelp help = HilecSingleton::hilec()->getUnitHelp(name);

    mStream << "<h1>" << name << "</h1>";
    mStream << "<p>" << help.desc << "</p>";

    if(!help.structs.empty())
        mStream << "<h2>Structs</h2>";
    foreach(const RcUnitHelp::Struct& s, help.structs)
    {
        mStream << "<p>" << s.name << "{";
        foreach(QString sig, s.members)
            mStream << "<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" << sig << ";";
        mStream << "<br />} </p>";
    }
    mStream << "<h2>Methods</h2>";
    foreach(const RcUnitHelp::Method& m, help.methods)
    {
        mStream << "<p><strong>" << m.sig << "</strong> " << m.shortDesc;
        mStream << "<br />" << m.longDesc << "</p> </hr />";
    }
}
