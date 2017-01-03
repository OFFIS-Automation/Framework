// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
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

#include "FilterSearchTree.h"
#include "FilterSearchTreeItem.h"
#include "FilterSearchWidget.h"
#include "../FilterConnect/FilterWidget.h"
#include "../FilterConnect/FilterConnectWidget.h"
#include <core/FilterInfo.h>
#include <QDebug>
#include <QApplication>

FilterSearchTree::FilterSearchTree(QWidget *parent) :
    QTreeWidget(parent)
{
    mDragTarget = 0;
}

void FilterSearchTree::mousePressEvent(QMouseEvent *event)
{
    FilterSearchTreeItem *selectedItem = dynamic_cast<FilterSearchTreeItem*>(itemAt(event->pos()));

    if (selectedItem)
    {
        mDragStart = event->pos();
    }
    QTreeWidget::mousePressEvent(event);
}

void FilterSearchTree::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
         return;
    if(mDragStart.isNull())
        return;
     if ((event->pos() - mDragStart).manhattanLength() < QApplication::startDragDistance())
         return;
    mDragStart = QPoint();
    FilterSearchTreeItem *selectedItem = dynamic_cast<FilterSearchTreeItem*>(itemAt(event->pos()));
    if (!selectedItem)
        return;
    FilterInfo info;
    info.typeInfo = selectedItem->getFilterType();
    info.name = info.typeInfo.name;
    FilterWidget filterWidget(info);
    QPoint hotspot = QPoint(100,10);
    if(mDragTarget)
        mDragTarget->startDrag(&filterWidget, hotspot);
}

void FilterSearchTree::mouseDoubleClickEvent(QMouseEvent *event)
{
    // Get selected item at double click position
    // (https://github.com/OFFIS-Automation/Framework/issues/4
    FilterSearchTreeItem *selectedItem = dynamic_cast<FilterSearchTreeItem*>(itemAt(event->pos()));

    // Add seleted item if not a node / nil
    if (!selectedItem)
        return;
    if(mDragTarget)
        mDragTarget->requestAddFilter(selectedItem->getFilterType().uid);

}
