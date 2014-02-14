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

#include "PerspectiveControl.h"
#include <QXmlStreamWriter>
#include <QSettings>
#include <QInputDialog>
#include <QMessageBox>
#include <QDockWidget>
#include "qglobal.h"

#include "MasterWindow.h"

PerspectiveControl::PerspectiveControl(MasterWindow *master, MainWindow *slave, QObject *parent) :
    QObject(parent), mMaster(master), mSlave(slave), mActionGroup(this), mElementMenu(0)
{
    if(mSlave == mMaster)
        mSlave = 0;

    mToolBar = new QToolBar(tr("Perspectives"));
    mToolBar->setIconSize(QSize(cButtonWidth, 26));
    mToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mToolBar->setObjectName("perspectiveToolbar");
    QAction* a = mToolBar->addAction(QIcon(":/img/newPerspective.png"), tr("New\nperspective"), this, SLOT(createPerspective()));
    a->setToolTip(tr("Create new perspective from current one"));
    mResetAction = mToolBar->addAction(QIcon(":/img/resetPerspective.png"), tr("Reset\nperspective"), this, SLOT(resetPerspectives()));
    mResetAction->setToolTip(tr("Reset current perspective"));
    mDeleteAction = mToolBar->addAction(QIcon(":/img/close.png"), tr("Delete\nperspective"), this, SLOT(deletePerspective()));
    mDeleteAction->setToolTip(tr("Delete the current perspective"));
    mToolBar->addSeparator();

    QSettings settings;
    settings.remove("perspectives");
    settings.remove("lastPerspective");
    settings.beginGroup("perspectiveSettings");
    QStringList names = settings.childGroups();
    foreach(QString name, names)
    {
        settings.beginGroup(name);
        Perspective& p = mPerspectives[name];
        p.masterObjects = settings.value("master/docks").toStringList();
        p.masterState = QByteArray::fromBase64(settings.value("master/state").toByteArray());
        p.slaveObjects = settings.value("slave/docks").toStringList();
        p.slaveState = QByteArray::fromBase64(settings.value("slave/state").toByteArray());
        settings.endGroup();
    }
    mDeleteAction->setEnabled(false);
    mResetAction->setEnabled(false);
    foreach(QString name, perspectives())
    {
        QAction* action = mToolBar->addAction(QIcon(":/img/usePerspective.png"), name, this, SLOT(perspectiveActionTriggered()));
        action->setCheckable(true);
        mActionGroup.addAction(action);
        mTriggers[name] = action;
    }
    reassignShortcuts();
#ifdef Q_OS_WIN
    addToolBar(Qt::LeftToolBarArea, mToolBar, "Default");
#else
    addToolBar(Qt::TopToolBarArea, mToolBar, "Default");
#endif
}

void PerspectiveControl::start()
{
    connect(this,SIGNAL(triggerStart()), SLOT(startDelayed()), Qt::QueuedConnection);
    emit triggerStart();
}

void PerspectiveControl::startDelayed()
{
    QSettings settings;
    QString lastPerspective = settings.value("perspectiveSettings/last").toString();
    if(!mTriggers.empty())
    {
        if(!mTriggers.contains(lastPerspective))
            lastPerspective = mTriggers.keys().first();
        mTriggers[lastPerspective]->trigger();
    }
    if(!mElementMenu)
        mElementMenu = mMaster->getMenu(tr("Widgets"));
    updateElements();
}

void PerspectiveControl::savePerspective(QString name)
{
    if(name.isEmpty())
        name = currentPerspective();
    if(name.isEmpty())
        return;
    Perspective& p = mPerspectives[name];
    p.masterObjects = mMaster->saveDocks();
    p.masterState = mMaster->saveState();
    if(mSlave)
    {
        p.slaveObjects = mSlave->saveDocks();
        p.slaveState = mSlave->saveState();
    }
    storePerspectives(name);
}

void PerspectiveControl::loadPerspective(const QString &name, bool saveCurrent)
{
    if(!mPerspectives.contains(name))
        return;
    if(saveCurrent)
        savePerspective();

    Perspective& p = mPerspectives[name];
    mMaster->setUpdatesEnabled(false);


    QDockWidget dummy1, dummy2;
    mMaster->setCentralDockWidget(&dummy1);
    if(mSlave)
    {
        mSlave->setUpdatesEnabled(false);
        mSlave->setCentralDockWidget(&dummy2);
    }

    mMaster->restoreDocks(p.masterObjects, mSlave, p.visibleObjects);
    if(mSlave)
        mSlave->restoreDocks(p.slaveObjects, mMaster, p.visibleObjects);

    mMaster->restoreState(p.masterState);
    if(mSlave)
        mSlave->restoreState(p.slaveState);
    mCurrenPerspective = name;
    QSettings settings;
    settings.setValue("perspectiveSettings/last", name);
    mMaster->setUpdatesEnabled(true);
    if(mSlave) mSlave->setUpdatesEnabled(true);
    mDeleteAction->setEnabled(!mPerspectives[name].fixed);
    mResetAction->setEnabled(mPerspectives[name].fixed);

}
void PerspectiveControl::resetPerspectives()
{
    QString key = currentPerspective();
    if(!mPerspectives.contains(key))
        return;
    Perspective& p = mPerspectives[key];
    if(!p.fixed)
        return;
    p.masterState.clear();
    p.slaveState.clear();
    p.masterObjects = p.defaultMasterObjects;
    p.slaveObjects = p.defaultSlaveObjects;
    storePerspectives(key);
    loadPerspective(key, false);
}

void PerspectiveControl::storePerspectives(const QString& perspective)
{
    QSettings settings;
    settings.beginGroup("perspectiveSettings");
    foreach(QString name, perspectives())
    {
        if(!perspective.isEmpty() && perspective != name)
            continue;
        const Perspective& p = mPerspectives[name];
        settings.beginGroup(name);
        settings.setValue("master/docks", p.masterObjects);
        settings.setValue("master/state", QString(p.masterState.toBase64()));

        settings.setValue("slave/docks", p.slaveObjects);
        settings.setValue("slave/state", QString(p.slaveState.toBase64()));
        settings.endGroup();
    }
}


void PerspectiveControl::createPerspective(QString name, bool trigger)
{
    if(name.isEmpty())
        name = QInputDialog::getText(mMaster, tr("Create Perspective"), tr("Enter name for the new perspective"));
    if(name.isEmpty() || mPerspectives.contains(name))
        return;
    savePerspective(name);
    QAction* action = mToolBar->addAction(QIcon(":/img/usePerspective.png"), name, this, SLOT(perspectiveActionTriggered()));
    action->setCheckable(true);
    mActionGroup.addAction(action);
    if(trigger)
        action->trigger();
    mTriggers[name] = action;
    reassignShortcuts();
}

void PerspectiveControl::deletePerspective()
{
    QString name = currentPerspective();
    if(!mPerspectives.contains(name))
        return;
    if(mPerspectives[name].fixed)
        return;
    if(QMessageBox::Yes != QMessageBox::question(mMaster, tr("Delete perspective"), tr("Delete perspective \"%1\"").arg(name), QMessageBox::Yes | QMessageBox::No))
        return;
    mPerspectives.remove(name);
    QSettings settings;
    settings.beginGroup("perspectiveSettings");
    settings.remove(name);
    QAction* trigger = mTriggers[name];
    mTriggers.remove(name);
    mToolBar->removeAction(trigger);
    delete trigger;
    mCurrenPerspective = QString();
    mTriggers.values().first()->trigger();
    reassignShortcuts();
}

void PerspectiveControl::perspectiveActionTriggered()
{
    QAction* from = qobject_cast<QAction*>(sender());
    if(!from)
        return;
    loadPerspective(from->text());
}

void PerspectiveControl::addElement(QWidget *widget, const QString& group)
{
    mElements[widget] = group;
    updateElements();
}

void PerspectiveControl::removeElement(QWidget *widget)
{
    mElements.remove(widget);
    updateElements();
}

void PerspectiveControl::updateElements()
{
    if(!mElementMenu)
        return;
    mElementMenu->clear();
    QMap<QString, QMap<QString, QList<QAction*> > > subMenus;
    foreach(QWidget* w, mElements.keys())
    {
        QDockWidget* dw = qobject_cast<QDockWidget*>(w);
        QToolBar* tb = qobject_cast<QToolBar*>(w);
        if(!dw && ! tb)
            continue;
        QString plugin = mElements[w];
        QString title = w->windowTitle();
        QAction* action = new QAction(title, this);
        connect(action, SIGNAL(triggered(bool)), w, SLOT(setVisible(bool)));
        action->setCheckable(true);
        action->setChecked(w->isVisible());
        if(dw)
            connect(dw, SIGNAL(visibilityChanged(bool)), action, SLOT(setChecked(bool)));
        else
            connect(tb, SIGNAL(visibilityChanged(bool)), action, SLOT(setChecked(bool)));
        subMenus[plugin][title] << action;
    }
    QStringList keys = subMenus.keys();
    keys.sort();
    foreach(QString key, keys)
    {
        QMenu* menu = new QMenu(key);
        mElementMenu->addMenu(menu);
        QStringList subKeys = subMenus[key].keys();
        subKeys.sort();
        foreach(QString subKey, subKeys)
        {
            menu->addActions(subMenus[key][subKey]);
        }
    }
}

void PerspectiveControl::reassignShortcuts()
{
    int key = Qt::Key_1;
    foreach(QAction* action, mActionGroup.actions())
    {
        action->setShortcut(key + Qt::ControlModifier);
        key++;
        if(key > Qt::Key_9)
            break;
    }
}

void PerspectiveControl::addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget, const QString& group)
{
    mMaster->addDockWidget(area, dockwidget);
    dockwidget->setVisible(false);
    addElement(dockwidget, group);
}

void PerspectiveControl::addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget, Qt::Orientation orientation, const QString& group)
{
    mMaster->addDockWidget(area, dockwidget, orientation);
    dockwidget->setVisible(false);
    addElement(dockwidget, group);
}

void PerspectiveControl::removeDockWidget(QDockWidget *dockwidget)
{
    mMaster->removeDockWidget(dockwidget);
    removeElement(dockwidget);
}

void PerspectiveControl::addToolBar(Qt::ToolBarArea area, QToolBar *toolbar, const QString& group)
{
    mMaster->addToolBar(area, toolbar);
    toolbar->setVisible(false);
    addElement(toolbar, group);
}

void PerspectiveControl::addToolBar(QToolBar *toolbar, const QString& group)
{
    mMaster->addToolBar(toolbar);
    toolbar->setVisible(false);
    addElement(toolbar, group);
}

QMenu& PerspectiveControl::getMenu(QString name, QStringList subMenuList)
{
    QMenu* menu = mMaster->getMenu(name);
    foreach(QString subMenuName, subMenuList)
    {
        bool found = false;
        foreach(QAction* action, menu->actions())
        {
            if(action->text() == subMenuName && action->menu() != 0)
            {
                found = true;
                menu = action->menu();
                break;
            }
        }
        if(!found)
            menu = menu->addMenu(subMenuName);
    }
    return *menu;
}

PerspectiveInterface& PerspectiveControl::getPerspective(const QString &perspective)
{
    if(!mPerspectives.contains(perspective))
    {
        createPerspective(perspective, false);
        mPerspectives[perspective].masterState.clear();
        mPerspectives[perspective].slaveState.clear();
    }
    mPerspectives[perspective].addToolbar(toolbar());
    mPerspectives[perspective].fixed = true;
    return mPerspectives[perspective];
}
