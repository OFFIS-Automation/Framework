// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
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

#ifndef MODELPLUGININTERFACE_H
#define MODELPLUGININTERFACE_H

class ModelPluginInterface
{
public:

    virtual QString getName() const = 0;
    virtual bool requirementsMet() = 0;
    virtual void initialize(const QString& loadDirectory) = 0;
    virtual void deinitialize() = 0;
    virtual QObject* getInterfaceObject() = 0;
};
#define ModelPluginInterface_iid "com.amir.toolkit.ModelInterface/1.0"
Q_DECLARE_INTERFACE(ModelPluginInterface, ModelPluginInterface_iid)

#endif // MODELPLUGININTERFACE_H
