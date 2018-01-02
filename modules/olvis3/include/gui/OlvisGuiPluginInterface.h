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

#ifndef OLVISGUIPLUGININTERFACE_H
#define OLVISGUIPLUGININTERFACE_H

#include <QString>
#include "PortEditInterface.h"
#include "OverlayInterface.h"
#include <core/OlvisInterface.h>

class OlvisGuiPluginInterface
{
public:
    virtual QString getName() const = 0;
    virtual PortEditInterface* portEditFor(const PortInfo& info) {
        Q_UNUSED(info)
        return 0;
    }
    virtual OverlayInterface* overlayFor(const QString& overlayName, bool isOutput, bool isMasterOverlay, OlvisInterface* visionInterface)
    {
        Q_UNUSED(overlayName)
        Q_UNUSED(isOutput)
        Q_UNUSED(isMasterOverlay)
        Q_UNUSED(visionInterface)
        return 0;
    }
    virtual OverlayInterface* overlayFor(const PortInfo& info, bool isOutput, bool isMasterOverlay, OlvisInterface* visionInterface)
    {
        Q_UNUSED(info)
        Q_UNUSED(isOutput)
        Q_UNUSED(isMasterOverlay)
        Q_UNUSED(visionInterface)
        return 0;
    }
};

#define OlvisGuiPluginInterface_iid "com.offis.vision.GuiPluginInterface/1.1"
Q_DECLARE_INTERFACE(OlvisGuiPluginInterface, OlvisGuiPluginInterface_iid)

#endif // OLVISGUIPLUGININTERFACE_H
