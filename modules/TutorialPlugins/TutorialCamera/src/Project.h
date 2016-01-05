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

#ifndef TEST_H
#define TEST_H

#include <filter/OlvisPlugin.h>

#if QT_VERSION >= 0x050000
class Test : public OlvisPlugin
{
    Q_OBJECT
    Q_INTERFACES(FilterPluginInterface)
    Q_PLUGIN_METADATA(IID FilterPluginInterface_iid)
};

#endif

#endif // TEST_H
