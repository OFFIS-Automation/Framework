// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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

#ifndef HILECPLUGIN_H
#define HILECPLUGIN_H

#include <QObject>
#include <interfaces/ModelPluginInterface.h>

#include "src/HilecCore.h"
#include "src/RcUnits.h"

class HilecPlugin : public QObject, public ModelPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(ModelPluginInterface)
    Q_PLUGIN_METADATA(IID ModelPluginInterface_iid)

public:
    explicit HilecPlugin();
    QString getName() const { return "Hilec"; }
    bool requirementsMet(){ return true; }
    void initialize(const QString& loadDirectory);
    void deinitialize();
    QObject* getInterfaceObject(){ return hilecCore; }
protected:
    HilecCore* hilecCore;
    RcUnits*  rcControl;

};

#endif // HILECPLUGIN_H
