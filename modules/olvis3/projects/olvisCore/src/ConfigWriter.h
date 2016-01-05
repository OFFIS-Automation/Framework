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

#ifndef CONFIGWRITER_H
#define CONFIGWRITER_H

#include <QString>
#include <QTextStream>
#include <QDir>

class QXmlStreamWriter;
class OlvisCoreInterface;
struct FilterInfo;

class ConfigWriter
{
public:
    ConfigWriter(const OlvisCoreInterface& model);
    QString toXml(const QString& targetFile);
    void toXml(QXmlStreamWriter* xml, const QString& targetFile);
protected:
    void addMakroFilter();
    void addProcessors();
    void addInputs(const FilterInfo& filter);
    void addJoins();
    void addBuffers();
    void addConnections();
    const OlvisCoreInterface& mInterface;
    QXmlStreamWriter* stream;
    QDir targetPath, comparePath;
};

#endif // CONFIGWRITER_H
