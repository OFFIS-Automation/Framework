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

#include "DockWidgetTitle.h"
#include "ui_DockWidgetTitle.h"
#include <QDockWidget>
#include <QDebug>
#include "MainWindow.h"


DockWidgetTitle::DockWidgetTitle(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::DockWidgetTitle)
{
    mDock = 0;
    mOther = 0;
    ui->setupUi(this);
    //ui->switchWindow->setVisible(false);
    QDockWidget* dock = qobject_cast<QDockWidget*>(parent);
    if(dock)
        connectToDock(dock);
}

QSize DockWidgetTitle::minimumSizeHint()
{
    return minimumSize();
}

DockWidgetTitle::~DockWidgetTitle()
{
    delete ui;
}

void DockWidgetTitle::connectToDock(QDockWidget *dock)
{
    if(mDock)
        return;
    mDock = dock;
    connect(dock, SIGNAL(featuresChanged(QDockWidget::DockWidgetFeatures)), SLOT(updateFeatures(QDockWidget::DockWidgetFeatures)));
    connect(ui->close, SIGNAL(clicked()), dock, SLOT(close()));
    ui->title->setText(dock->windowTitle());
    updateFeatures(dock->features());
}

void DockWidgetTitle::on_switchFloat_clicked()
{
    mDock->setFloating(!mDock->isFloating());
}

void DockWidgetTitle::on_switchWindow_clicked()
{
    if(!mOther)
        return;
    MainWindow* current = qobject_cast<MainWindow*>(mDock->parentWidget());
    MainWindow* target = mOther;
    if(current)
    {
        Qt::DockWidgetArea area = current->dockWidgetArea(mDock);
        mDock->hide();
        current->removeDockWidget(mDock);
        target->addDockWidget(area, mDock);
        mOther = current;
        mDock->show();
        if(target->centralDockWidget() == 0)
            ui->makeCentral->click();
    }

}

void DockWidgetTitle::setOtherWindow(MainWindow *window)
{
    mOther = window;
    updateFeatures(mDock->features());
}

void DockWidgetTitle::updateFeatures(QDockWidget::DockWidgetFeatures features)
{
    ui->close->setVisible(features & QDockWidget::DockWidgetClosable);
    ui->switchFloat->setVisible(features & QDockWidget::DockWidgetFloatable);
    MainWindow* current = qobject_cast<MainWindow*>(mDock->parentWidget());
    bool isCentral = current == 0;
    ui->switchWindow->setVisible(!isCentral && mOther);
    ui->makeCentral->setVisible(!isCentral);
}

void DockWidgetTitle::on_makeCentral_clicked()
{
    MainWindow* current = qobject_cast<MainWindow*>(mDock->parentWidget());
    current->setCentralDockWidget(mDock);
}
