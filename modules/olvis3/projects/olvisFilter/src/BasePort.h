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

#ifndef BASEPORT_H
#define BASEPORT_H
#include <core/PortInfo.h>
#include <QtGui/QPixmap>

class BasePort
{
public:
    BasePort(int typeId, const QString& typeName, bool isArray, const QString& defaultName);
    void setName(const QString& name);
    void setDesc(const QString& desc);
    void setIcon(const QImage &icon);
    bool isSetupMode() const;
    void endSetupMode();
    const PortInfo& getInfo() const { return mInfo; }
    QString name() const { return mInfo.name; }
    void setContraint(const QByteArray& key, const QVariant& value);
    void addCompatiblePort(int typeId, bool warnOnConnect, const QString& warnMsg = QString());
    void setIsMainType(bool main);
    void setMode(InputPortMode mode);
protected:
    PortInfo mInfo;
    bool mSetupMode;
};

#endif // BASEPORT_H
