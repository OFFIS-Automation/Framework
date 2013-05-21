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

#ifndef DEFAULTPORTS_H
#define DEFAULTPORTS_H

#include <QObject>
#include <gui/OlvisGuiPluginInterface.h>

class DefaultPorts : public QObject, public OlvisGuiPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(OlvisGuiPluginInterface)
    Q_PLUGIN_METADATA(IID OlvisGuiPluginInterface_iid)
public:
    explicit DefaultPorts();
    virtual QString getName() const { return "DefaultPorts"; }
    virtual PortEditInterface* portEditFor(const PortInfo& info);
signals:

public slots:

};

#endif // DEFAULTPORTS_H
