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


#ifndef TUTORIALCORE_H
#define TUTORIALCORE_H

#include "GraphicsView.h"

#include <interfaces/GuiPluginInterface.h>

class TutorialCore : public QObject, public GuiPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(GuiPluginInterface)
    Q_PLUGIN_METADATA(IID GuiPluginInterface_iid)
public:
    TutorialCore();
    virtual ~TutorialCore();
    virtual QString getName() const { return "SensorSystemGui"; }
    virtual void setModelInterface(const QString& /*name */, QObject* /* modelInterface */) {}
    virtual bool requirementsMet() { return true; }
    virtual void initialize(const QString& );
    virtual void deinitialize(){}
    virtual void setGuiInterface(const QString& /*name */, QObject* /* connectObject*/) {}
    virtual QObject* getConnectObject() { return this; }
    virtual void addElements(MainWindowInterface* /* mainWindow */) {}
    virtual void start() {}
protected:
    GraphicsView mScene;
};

#endif // TUTORIALCORE_H
