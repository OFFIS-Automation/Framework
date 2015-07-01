// OFFIS Automation Framework
// Copyright (C) 2013-2014 OFFIS e.V.
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

#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <QXmlStreamReader>
#include <QMap>
class OlvisInterface;
class ConfigReader : QXmlStreamReader
{
public:
    ConfigReader(OlvisInterface& control, const QString& data);
    ConfigReader(OlvisInterface& control, QIODevice* device);
    void createConfig();
protected:
    void createMakroFilter(bool local = true);
    void createMakroInput();
    void createMakroOutput();
    void createProcessor();
    void createBuffer();
    void createJoin();
    void createFilter();
    void setPort();
    void addProcessorOutput();
    void addMakroOutput();
    void addMakroInput();
    void setSource();
    void setValue();
    void connectProcessors();
    void connectProcessorInput();

    int mCurrentProcessor;
    int mCurrentFilter;
    QString mCurrentPort;
    QString mCurrentData;
    QMap<QString, int> mProcessingElementIds;

    OlvisInterface& mInterface;
};

#endif // CONFIGREADER_H
