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

#ifndef PROJECTEDITOR_H
#define PROJECTEDITOR_H

#include <interfaces/GuiPluginInterface.h>
#include <core/HilecInterface.h>
#include "EditorToolBar.h"
#include "FileTree/FileTree.h"
#include "Editor/EditArea.h"
#include "search.h"


class ProjectEditor : public QObject, public GuiPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(GuiPluginInterface)
    Q_PLUGIN_METADATA(IID GuiPluginInterface_iid)
public:
    explicit ProjectEditor(QObject *parent = 0);
    virtual QString getName() const;
    virtual void setModelInterface(const QString& name, QObject* modelInterface);
    virtual bool requirementsMet();
    virtual void initialize(const QString& pluginDir);
    virtual void deinitialize();
    virtual void setGuiInterface(const QString& name, QObject* connectObject);
    virtual QObject* getConnectObject();
    virtual void addElements(MainWindowInterface* mainWindow);
    virtual void start();
    void loadProject(const QString& projectName);
    void closeProject();

signals:
    void activeFileChanged(const QString& filename);
    void fileSaved(const QString& filename);
    void fileOpened(const QString& filename);

public slots:
    void focusLine(const QString& file, int line);

protected:
    // add window objects here
    HilecInterface* mHilec;
    bool mHasOlvis;
    EditorToolBar *mToolbar;
    FileTree *mFileTree;
    EditArea *mEditArea;

private:
    QString mBaseDir;
    QString mProjectName;
};

#endif // PROJECTEDITOR_H
