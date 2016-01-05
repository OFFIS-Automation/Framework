// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

#ifndef CONFIGURATIONSLISTWIDGET_H
#define CONFIGURATIONSLISTWIDGET_H

#include <QListWidget>

class ConfigurationListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ConfigurationListWidget(QWidget *parent = 0);

signals:
    void fileDropped(const QString& path);
public slots:

protected:
    QString extractFileFromDrag(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *event);
};

#endif // CONFIGURATIONSLISTWIDGET_H
