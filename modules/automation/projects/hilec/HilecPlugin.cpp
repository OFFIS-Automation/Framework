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

#include "HilecPlugin.h"
#include <QtPlugin>
#include "src/OlvisSingleton.h"

HilecPlugin::HilecPlugin()
{
}

void HilecPlugin::initialize(const QString &pluginDir)
{
    rcControl = new RcUnits(pluginDir + "/rcUnits");
    hilecCore = new HilecCore(pluginDir + "/hilec");
}

void HilecPlugin::deinitialize()
{
    delete hilecCore;
    delete rcControl;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(HilecPlugin, HilecPlugin)
#endif
