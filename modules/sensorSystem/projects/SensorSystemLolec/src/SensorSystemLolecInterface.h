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

#ifndef SENSORSYSTEMLOLECINTERFACE_H
#define SENSORSYSTEMLOLECINTERFACE_H

#include <lolecs/LolecInterface.h>

class SensorSystemLolecInterface : public QObject, public LolecInterface
{
public:
    Q_OBJECT
    Q_INTERFACES(LolecInterface)
    Q_PLUGIN_METADATA(IID LolecInterface_iid)
public:
    explicit SensorSystemLolecInterface();
    virtual bool needsConfigFile() const { return false; }
    virtual QString name() const { return "SensorSystem and Trace export plugin"; }
    virtual QString description() const;
    QObject* instance(RcBase &rc, const QString &configFile, const QString &name);
    QWidget* guiForInstance(QObject *){ return 0; }
};

#endif // SENSORSYSTEMLOLECINTERFACE_H
