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

#include "ConfigurationListWidget.h"
#include <QDragEnterEvent>
#include <QDebug>
#include <QUrl>
#include <QMimeData>

ConfigurationListWidget::ConfigurationListWidget(QWidget *parent) :
    QListWidget(parent)
{
}

void ConfigurationListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(extractFileFromDrag(event).isEmpty())
        QListWidget::dragEnterEvent(event);
}

void ConfigurationListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if(extractFileFromDrag(event).isEmpty())
        QListWidget::dragMoveEvent(event);
}

void ConfigurationListWidget::dropEvent(QDropEvent *event)
{
    QString filename = extractFileFromDrag(event);
    if(!filename.isEmpty())
    {
        qDebug() << filename;
        emit fileDropped(filename);
    }
    else
        QListWidget::dropEvent(event);
}

QString ConfigurationListWidget::extractFileFromDrag(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();
    if(mimeData->hasUrls())
    {
        foreach(QUrl url, mimeData->urls())
        {
            if(url.scheme() == "file")
            {
                event->setDropAction(Qt::CopyAction);
                event->accept();
                return url.toLocalFile();
            }
        }
    }
    return QString();
}


