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

#ifndef SCRIPTEXCEPTION_H
#define SCRIPTEXCEPTION_H

#include <QString>
#include <QList>

struct TraceLine
{
    QString file;
    int line;
    QString module;
    QString method;
};

struct ScriptException
{
    typedef TraceLine Trace;
    QString name;
    QString baseDir;
    QList<Trace> trace;
};

struct ScriptCompileProblem
{
    enum Mode
    {
        Error,
        Warning,
        Info
    } mode;
    int line;
    QString msg;
};

struct ScriptCompileInfo
{
    QString file;
    QList<ScriptCompileProblem> problems;
};

#endif // SCRIPTEXCEPTION_H
