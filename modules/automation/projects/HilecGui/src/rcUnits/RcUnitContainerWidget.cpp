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

#include "RcUnitContainerWidget.h"
#include "ui_RcUnitContainerWidget.h"
#include "../HilecSingleton.h"
#include <QDebug>
#include <QGroupBox>

RcUnitContainerWidget::RcUnitContainerWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::RcUnitContainerWidget)
{
    ui->setupUi(this);
    splitter = 0;
    mOrientation = Qt::Horizontal;
    connect(HilecSingleton::hilec(), SIGNAL(rcUnitsChanged(bool)), SLOT(updateRcUnits(bool)));
    connect(this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), SLOT(onDockLocationChanged(Qt::DockWidgetArea)));
}

RcUnitContainerWidget::~RcUnitContainerWidget()
{
    delete ui;
}

void RcUnitContainerWidget::onDockLocationChanged(const Qt::DockWidgetArea &area)
{
    if(area == Qt::LeftDockWidgetArea || area == Qt::RightDockWidgetArea)
        mOrientation = Qt::Vertical;
    else
        mOrientation = Qt::Horizontal;

    if(splitter)
        splitter->setOrientation(mOrientation);
}

void RcUnitContainerWidget::updateRcUnits(bool partial)
{
    if(partial)
        return;

    if(splitter)
    {
        ui->layout->removeWidget(splitter);
        delete splitter;
    }
    splitter = new QSplitter(mOrientation, this);
    ui->layout->addWidget(splitter);


    HilecInterface* hilec = HilecSingleton::hilec();
    foreach(const QString& name, hilec->rcUnits())
    {
        QWidget* w = hilec->createLolecWidget(name);
        if(w)
        {
            QGroupBox* box = new QGroupBox(name);
            box->setLayout(new QHBoxLayout());
            box->layout()->addWidget(w);
            splitter->addWidget(box);
        }
    }
    splitter->addWidget(new QWidget());
}
