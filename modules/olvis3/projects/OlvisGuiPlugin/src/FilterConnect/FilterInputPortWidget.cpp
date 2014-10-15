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

#include "FilterInputPortWidget.h"
#include "../PluginContainer.h"
#include "../OlvisGuiPlugin.h"
#include <core/OlvisInterface.h>
#include <QDialog>
#include <QVBoxLayout>
#include "../OlvisSingleton.h"

FilterInputPortWidget::FilterInputPortWidget(int filterId, const PortInfo &info, QWidget *parent)
    : FilterPortWidget(filterId, info, parent)
{
    setStyleSheet("background-color: #a0a0f0;");
    mEdit= PluginContainer::getInstance().portEditFor(info);
    if(mEdit)
    {
        mEdit->setInfo(mInfo);
    }
}

FilterInputPortWidget::~FilterInputPortWidget()
{
    if(mEdit)
        delete mEdit;
}

void FilterInputPortWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    if(!mEdit)
        return;

    mEdit->setValue(OlvisSingleton::instance().getPortValue(filterId(), portName()));
    QWidget* w = mEdit->editWidget();
    if(w)
    {
        w->setWindowModality(Qt::ApplicationModal);
        w->setWindowTitle(tr("%1: Set value").arg(portName()));
        w->setWindowIcon(QPixmap::fromImage(mInfo.icon));
        w->setMinimumWidth(250);
        w->show();
    }
    mEdit->startEdit(*this);
}

void FilterInputPortWidget::editFinished(const QVariant &variant)
{
    editCanceled();
    emit newPortValue(filterId(), portName(), variant);
}

void FilterInputPortWidget::editCanceled()
{
    if(!mEdit)
        return;
    QWidget* w = mEdit->editWidget();
    if(w)
    {
        w->hide();
    }
}
