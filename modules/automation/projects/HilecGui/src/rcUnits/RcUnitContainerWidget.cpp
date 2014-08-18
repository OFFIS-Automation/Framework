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
#include "RcUnitFlagWidget.h"
#include "ui_RcUnitContainerWidget.h"
#include "../HilecSingleton.h"
#include <QDebug>
#include <QGroupBox>
#include <core/RcUnitHelp.h>

RcUnitContainerWidget::RcUnitContainerWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::RcUnitContainerWidget)
{
    ui->setupUi(this);
    splitter = 0;
    mOrientation = Qt::Horizontal;
    connect(HilecSingleton::hilec(), SIGNAL(rcUnitsChanged(bool)), SLOT(updateRcUnits(bool)));
    connect(HilecSingleton::hilec(), SIGNAL(rcUnitFlagsUpdated(QString,QVariantList)), SLOT(unitFlagsUpdated(QString,QVariantList)));
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
    {
        HilecInterface* hilec = HilecSingleton::hilec();
        foreach(const QString& name, hilec->rcUnits())
        {
            RcUnitFlagWidget* flagWidget = mFlagWidgets.value(name, 0);
            if(flagWidget)
            {
                RcUnitHelp help = hilec->getUnitHelp(name);
                flagWidget->unitStatusChanged(help.hwConnected);
            }
        }
    }

    if(splitter)
    {
        ui->layout->removeWidget(splitter);
        delete splitter;
    }
    mFlagWidgets.clear();
    splitter = new QSplitter(mOrientation, this);
    ui->layout->addWidget(splitter);


    HilecInterface* hilec = HilecSingleton::hilec();
    foreach(const QString& name, hilec->rcUnits())
    {
        RcUnitHelp help = hilec->getUnitHelp(name);
        if(!help.flags.empty())
        {
            RcUnitFlagWidget* w = new RcUnitFlagWidget(help);
            mFlagWidgets[name] = w;
            splitter->addWidget(w);
        }
    }
    splitter->addWidget(new QWidget());
}

void RcUnitContainerWidget::unitFlagsUpdated(const QString &unitName, const QVariantList &flags)
{
    RcUnitFlagWidget* flagWidget = mFlagWidgets.value(unitName, 0);
    if(flagWidget)
        flagWidget->updateFlags(flags);
}
