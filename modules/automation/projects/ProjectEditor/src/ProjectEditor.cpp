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

#include "ProjectEditor.h"

#include "HilecSingleton.h"

#include <QCoreApplication>
#include <QSettings>
#include <QtPlugin>
#include <QXmlStreamWriter>

ProjectEditor::ProjectEditor(QObject *parent) :
    QObject(parent)
{
    mHilec = 0;
    mHasOlvis = false;
}

QString ProjectEditor::getName() const
{
    return "ProjectEditor";
}

void ProjectEditor::setModelInterface(const QString& name, QObject* object)
{
    if(!mHilec && name == "Hilec")
    {
        mHilec = static_cast<HilecInterface*>(object);
        HilecSingleton::setHilec(mHilec);
    }
    if(name == "OlvisCore")
        mHasOlvis = true;
}

bool ProjectEditor::requirementsMet()
{
    return true;
}

void ProjectEditor::initialize(const QString&)
{
    // create widgets here
    mToolbar = new EditorToolBar();
    mFileTree = new FileTree;
    mEditArea = new EditArea;

    connect(mToolbar, SIGNAL(newFileRequested(QString)), mEditArea, SLOT(newFile(QString)));
    connect(mToolbar, SIGNAL(openFileRequested(QString)), mEditArea, SLOT(openFile(QString)));
    connect(mToolbar, SIGNAL(saveFileRequested()), mEditArea, SLOT(saveFile()));
    connect(mToolbar, SIGNAL(saveAllRequested()), mEditArea, SLOT(saveAll()));

    connect(mToolbar, SIGNAL(tabRequested()), mEditArea, SLOT(setTabView()));
    connect(mToolbar, SIGNAL(cascadeRequested()), mEditArea, SLOT(cascade()));
    connect(mToolbar, SIGNAL(tileRequested()), mEditArea, SLOT(tile()));

    connect(mToolbar, SIGNAL(closeCurrentFileRequested()), mEditArea, SLOT(closeCurrent()));
    connect(mToolbar, SIGNAL(closeAllFilesRequested()), mEditArea, SLOT(closeAll()));
    connect(mToolbar, SIGNAL(findRequested()), mEditArea, SLOT(showFind()));

    connect(mToolbar, SIGNAL(undoRequested()), mEditArea, SLOT(undo()));
    connect(mToolbar, SIGNAL(redoRequested()), mEditArea, SLOT(redo()));

    connect(mToolbar, SIGNAL(zoomInRequested()), mEditArea, SLOT(increaseFontSize()));
    connect(mToolbar, SIGNAL(zoomOutRequested()), mEditArea, SLOT(decreaseFontSize()));
    connect(mToolbar, SIGNAL(zoomNormalRequested()), mEditArea, SLOT(setFontSize()));
    connect(mToolbar, SIGNAL(copyRequested()), mEditArea, SLOT(copy()));
    connect(mToolbar, SIGNAL(cutRequested()), mEditArea, SLOT(cut()));
    connect(mToolbar, SIGNAL(pasteRequested()), mEditArea, SLOT(paste()));

    connect(mFileTree, SIGNAL(openFileRequested(QString)), mEditArea, SLOT(openFile(QString)));
    connect(mFileTree, SIGNAL(newFileRequested(QString)), mEditArea, SLOT(newFile(QString)));
    connect(mFileTree, SIGNAL(closeFileRequested(QString)), mEditArea, SLOT(closeFile(QString)));
    connect(mFileTree, SIGNAL(fileRenamed(QString,QString)), mEditArea, SLOT(renameFile(QString, QString)));
    connect(mFileTree, SIGNAL(directoryRenamed(QString,QString)), mEditArea, SLOT(renameDir(QString, QString)));

    connect(mEditArea, SIGNAL(currentFileChanged(QString)), SIGNAL(activeFileChanged(QString)));
    connect(mEditArea, SIGNAL(fileSaved(QString)), SIGNAL(fileSaved(QString)));
    connect(mEditArea, SIGNAL(fileOpened(QString)), SIGNAL(fileOpened(QString)));
    connect(mEditArea, SIGNAL(clickedProblem(QString,int)), SIGNAL(clickedProblem(QString,int)));

    connect(mEditArea, SIGNAL(undoStatusChanged(bool)), mToolbar, SLOT(onUndoStatusChanged(bool)));
    connect(mEditArea, SIGNAL(redoStatusChanged(bool)), mToolbar, SLOT(onRedoStatusChanged(bool)));
    connect(mEditArea, SIGNAL(cutCopyStatusChanged(bool)), mToolbar, SLOT(onCutCopyStatusChanged(bool)));

    closeProject();

}

void ProjectEditor::deinitialize()
{
}

void ProjectEditor::setGuiInterface(const QString& name, QObject* obj)
{
    if(name == "HilecGui")
    {
        mEditArea->connect(obj, SIGNAL(focusLine(QString, int)), SLOT(focusLine(QString, int)));
    }
}


QObject* ProjectEditor::getConnectObject()
{
    return this;
}

void ProjectEditor::addElements(MainWindowInterface* mainWindow)
{
    // add QDockWidgets to the main Window here
    mainWindow->addToolBar(mToolbar, tr("Scripting"));
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, mFileTree, tr("Scripting"));
    mainWindow->addDockWidget(Qt::RightDockWidgetArea, mEditArea, tr("Scripting"));

    mToolbar->initMenu(&mainWindow->getMenu(tr("&File")));

    PerspectiveInterface& perspective = mainWindow->getPerspective(tr("Automation"));
    perspective.addToolbar(mToolbar);
    perspective.setCentralWidget(mEditArea);
    perspective.addDockWidget(mFileTree);

    PerspectiveInterface& unitPerspective = mainWindow->getPerspective(tr("RC-Units"));
    unitPerspective.addToolbar(mToolbar);
    unitPerspective.setCentralWidget(mEditArea);
    unitPerspective.addDockWidget(mFileTree);

    PerspectiveInterface& debugPerspective = mainWindow->getPerspective(tr("Debug"));
    debugPerspective.addToolbar(mToolbar);
    debugPerspective.setCentralWidget(mEditArea);
    debugPerspective.addDockWidget(mFileTree);
}



void ProjectEditor::start()
{
}


void ProjectEditor::loadProject(const QString &projectName)
{
    mProjectName = projectName;
    mBaseDir = QFileInfo(projectName).absolutePath();
    mToolbar->setBaseDir(mBaseDir);
    mFileTree->updateTree(mBaseDir);
    mFileTree->setEnabled(true);
    //mToolbar->onProjectOpened(projectName);
    mEditArea->setBaseDir(mBaseDir);
}

void ProjectEditor::closeProject()
{
    mEditArea->saveAll();
    mEditArea->closeAll();
    mProjectName.clear();
    mBaseDir.clear();
    mFileTree->clear();
    mFileTree->setEnabled(false);
    mToolbar->setBaseDir(QString());
}
