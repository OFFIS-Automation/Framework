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

#ifndef FILTERPLUGIN_H
#define FILTERPLUGIN_H

#include <QtPlugin>
#include <QList>

class InputPort;
class OutputPort;
class SrcFilter;

class FilterPluginInterface
{
public:
    virtual int numFilters() = 0;
    virtual SrcFilter* createFilter(int num) = 0;
};

#define FilterPluginInterface_iid "com.offis.vision.FilterPluginInterface/1.0"

Q_DECLARE_INTERFACE(FilterPluginInterface, FilterPluginInterface_iid)

#if QT_VERSION < 0x050000
#define Q_PLUGIN_METADATA(x)
#endif

#endif // FILTERPLUGIN_H
