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

#include "Perspective.h"
#include <QToolBar>
#include <QDockWidget>

void Perspective::addToolbar(QToolBar *bar)
{
    visibleObjects << bar->objectName();
}

void Perspective::setCentralWidget(QDockWidget *w, int screen)
{
    if(screen == 0)
        updateList(defaultMasterObjects, w, 0);
    else
        updateList(defaultSlaveObjects, w, 0);
}

void Perspective::addDockWidget(QDockWidget *w, int screen)
{
    if(screen == 0)
        updateList(defaultMasterObjects, w);
    else
        updateList(defaultSlaveObjects, w);
}

void Perspective::updateList(QStringList &list, QObject *object, int pos)
{
    list.insert(pos, object->objectName());
    list.removeDuplicates();
    visibleObjects << object->objectName();
    if(isFresh())
    {
        masterObjects = defaultMasterObjects;
        slaveObjects = defaultSlaveObjects;
    }
}
