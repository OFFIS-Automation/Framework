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

#ifndef FILTERSEARCHTREE_H
#define FILTERSEARCHTREE_H

#include <QWidget>
#include <QTreeWidget>
#include <QMimeData>
#include <QDrag>
#include <QListWidgetItem>
class FilterConnectWidget;

class FilterSearchTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit FilterSearchTree(QWidget *parent = 0);
    void setDragTarget(FilterConnectWidget* dragTarget) { mDragTarget = dragTarget; }
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    QListWidgetItem* parentListWidgetItem(QListWidgetItem *selectedItem);
    FilterConnectWidget* mDragTarget;
signals:

public slots:

private:
    QPoint mDragStart;
};

#endif // FILTERSEARCHTREE_H
