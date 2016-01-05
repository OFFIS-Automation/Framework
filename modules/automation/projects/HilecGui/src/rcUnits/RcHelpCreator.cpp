// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

#include "../HilecSingleton.h"


RcHelpCreator::RcHelpCreator(const QString &name)
{
    mStream.setString(&mBuffer, QIODevice::WriteOnly);

    RcUnitHelp help = HilecSingleton::hilec()->getUnitHelp(name);

    mStream << "<h1>" << name << "</h1>";
    mStream << "<p>" << help.desc << "</p>";

    if(!help.constants.empty())
    {
        mStream << "<h2>" << tr("Constants", "Programming") << "</h2>\n<p>";
        foreach(const QString& key, help.constants.keys())
        {
            QString value = help.constants[key].toString();
            mStream << key << " = " << value << "<br />";
        }
        mStream << "</p>";
    }
    if(!help.structs.empty())
        mStream << "<h2>" << tr("Structs", "Programming") << "</h2>";
    foreach(const RcUnitHelp::Struct& s, help.structs)
    {
        mStream << "<p>" << s.name << "{";
        foreach(QString sig, s.members)
            mStream << "<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" << sig << ";";
        mStream << "<br />} </p>";
    }
    mStream << "<h2>" << tr("Methods", "Programming") << "</h2>";

    // restructure methods due to overloading
    QMap<QString, OverloadedMethod> newStructure;
    foreach(const RcUnitHelp::Method& m, help.methods)
    {
        if(!newStructure.contains(m.name))
            newStructure[m.name].method = m;
        else
            newStructure[m.name].overloadedSignatures << m.sig;
    }
    foreach(const OverloadedMethod& m, newStructure)
    {
        mStream << "<p><strong>" << m.method.sig;
        foreach(QString sig, m.overloadedSignatures)
            mStream << ",<br />" << sig;

        mStream << "</strong> "<< m.method.shortDesc;
        mStream << "<br />" << m.method.longDesc << "</p> </hr />";
    }
}
