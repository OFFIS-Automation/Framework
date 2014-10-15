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

#ifndef OLVISLOLECINTERFACE_H
#define OLVISLOLECINTERFACE_H

#include <lolecs/LolecInterface.h>

class OlvisLolecInterface : public QObject, public LolecInterface
{
    Q_OBJECT
    Q_INTERFACES(LolecInterface)
    Q_PLUGIN_METADATA(IID LolecInterface_iid)
public:
    explicit OlvisLolecInterface();
    virtual bool needsConfigFile() const { return false; }
    virtual QString name() const { return "OLVIS automation plugin"; }
    virtual QString description() const;
    QObject* instance(RcBase &rc, const QString &configFile, const QString &name);
    QWidget* guiForInstance(QObject *){ return 0; }

};

#endif // OLVISLOLECINTERFACE_H
