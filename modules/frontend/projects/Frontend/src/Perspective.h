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

#ifndef PERSPECTIVE_H
#define PERSPECTIVE_H

#include "interfaces/GuiPluginInterface.h"

class Perspective: public PerspectiveInterface
{
public:
    Perspective() { fixed = false; displayedOnce = false; }
    virtual void addToolbar(QToolBar* bar);
    virtual void setCentralWidget(QDockWidget* w, int screen = 0);
    virtual void addDockWidget(QDockWidget* w, int screen = 0);
    bool isFresh() { return masterState.isEmpty(); }
    QByteArray masterState, slaveState;
    QStringList masterObjects, slaveObjects, visibleObjects, defaultMasterObjects, defaultSlaveObjects;
    bool fixed, displayedOnce;
protected:
    void updateList(QStringList& list, QObject* object, int pos = 1);
};

#endif // PERSPECTIVE_H
