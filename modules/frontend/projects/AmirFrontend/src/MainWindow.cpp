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

#include "MainWindow.h"
#include <QMouseEvent>
#include "LogWindow.h"
#include "DockWidgetTitle.h"
#include <interfaces/logging.h>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent, bool setupCentral) :
    QMainWindow(parent) , mOther(0)
{
    setWindowTitle("OFFIS Automation Framework");
    setWindowIcon(QIcon(":/img/icon.ico"));
    mCentral = new QWidget();
    mLayout = new QVBoxLayout();
    mCentral->setLayout(mLayout);
    //mLayout->setContentsMargins(0,0,0,0);
    if(setupCentral)
        setCentralWidget(mCentral);
    setDockNestingEnabled(true);
}

MainWindow::~MainWindow()
{
}

void MainWindow::addDockWidget(Qt::DockWidgetArea area, QDockWidget *child)
{
    QMainWindow::addDockWidget(area, child);
    updateDockWidget(child);
}

void MainWindow::addDockWidget(Qt::DockWidgetArea area, QDockWidget *child, Qt::Orientation orientation)
{
    QMainWindow::addDockWidget(area, child, orientation);
    updateDockWidget(child);
}

void MainWindow::addCentralDockWidget(Qt::DockWidgetArea area, QDockWidget *child)
{
    updateDockWidget(child);
    QMainWindow::addDockWidget(area, child);
    if(!centralDockWidget())
        setCentralDockWidget(child);
}

void MainWindow::removeDockWidget(QDockWidget *dockwidget)
{
    QMainWindow::removeDockWidget(dockwidget);
}

void MainWindow::setAlternative(MainWindow *window)
{
    mOther = window;
    QList<QDockWidget*> children = findChildren<QDockWidget*>();
    foreach(QDockWidget* child, children)
    {
        updateDockWidget(child);
    }
    if(window)
        connect(window, SIGNAL(aboutToClose()), SLOT(close()));
}

void MainWindow::addToolBar(Qt::ToolBarArea area, QToolBar *toolbar)
{
    QMainWindow::addToolBar(area, toolbar);
}

void MainWindow::addToolBar(QToolBar *toolbar)
{
    QMainWindow::addToolBar(toolbar);
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    emit aboutToClose();
    QMainWindow::closeEvent(ev);
}

QDockWidget* MainWindow::centralDockWidget()
{
    if(mLayout->count() > 0)
        return qobject_cast<QDockWidget*>(mLayout->itemAt(0)->widget());
    return 0;
}

void MainWindow::setCentralDockWidget(QDockWidget *dock)
{
    Qt::DockWidgetArea area = dockWidgetArea(dock);
    if(area == Qt::NoDockWidgetArea)
        area = Qt::LeftDockWidgetArea;
    removeDockWidget(dock);
    mLayout->addWidget(dock);
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    if(mLayout->count() > 1)
    {
        QDockWidget* current = qobject_cast<QDockWidget*>(mLayout->itemAt(0)->widget());
        if(current && current != dock)
        {
            mLayout->removeWidget(current);
            addDockWidget(area, current);
            current->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
            current->setVisible(true);
        }
    }
    dock->setVisible(true);
}

void MainWindow::restoreDocks(QStringList docks, MainWindow* other, QStringList visibleObjects)
{
    if(docks.size() == 0)
        return;
    QString dockName = docks.takeFirst();
    visibleObjects << dockName; // make sure central dock is visible
    QDockWidget* centralDock = takeoverDock(dockName, other);
    if(centralDock)
    {

        setCentralDockWidget(centralDock);
        centralDock->setVisible(true);
        DockWidgetTitle* title = qobject_cast<DockWidgetTitle*>(centralDock->titleBarWidget());
        if(title) title->setOtherWindow(other);
    }
    foreach(dockName, docks)
    {
        QDockWidget* dock = takeoverDock(dockName, other);
        if(dock)
        {
            DockWidgetTitle* title = qobject_cast<DockWidgetTitle*>(dock->titleBarWidget());
            if(title) title->setOtherWindow(other);
        }
    }
    foreach(QToolBar* bar, findChildren<QToolBar*>())
    {
        bar->setVisible(visibleObjects.contains(bar->objectName()));
    }
    foreach(QDockWidget* w, findChildren<QDockWidget*>())
    {
        w->setVisible(visibleObjects.contains(w->objectName()));
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    QMainWindow::keyPressEvent(e);
}


QStringList MainWindow::saveDocks()
{
    QStringList elements;
    QWidget* central = centralDockWidget();
    if(central)
        elements << central->objectName();
    QList<QDockWidget*> docks = findChildren<QDockWidget*>();
    foreach(QDockWidget* dock, docks)
    {
        if(dock != central)
            elements << dock->objectName();
    }
    return elements;
}

QDockWidget* MainWindow::takeoverDock(const QString& name, MainWindow *other)
{
    // is it mine?
    QDockWidget* dock = findChild<QDockWidget*>(name);
    if(dock)
    {
        //removeDockWidget(dock);
        return dock;
    }
    // its not mine
    if(other)
    {
        dock = other->findChild<QDockWidget*>(name);
        if(!dock)
            return 0;
        Qt::DockWidgetArea area = other->dockWidgetArea(dock);
        other->removeDockWidget(dock);
        addDockWidget(area, dock);
        return dock;
    }
    return 0;

}

void MainWindow::updateDockWidget(QDockWidget *child)
{
    child->setAllowedAreas(Qt::AllDockWidgetAreas);
    child->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
    DockWidgetTitle* title = qobject_cast<DockWidgetTitle*>(child->titleBarWidget());
    if(!title)
    {
        title = new DockWidgetTitle(child);
        child->setTitleBarWidget(title);
    }
    title->setOtherWindow(mOther);
}
