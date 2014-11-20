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

#include <QtGui>
#include <QLabel>
#include <QGraphicsItem>

#include "ProcessorsWidget.h"
#include "ui_ProcessorsWidget.h"

#include "ChangeProcessorDialog.h"

#include <src/OlvisSingleton.h>

ProcessorsWidget::ProcessorsWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ProcessorsWidget)
{
    ui->setupUi(this);

    scene = new DiagramScene(this);
    scene->setSceneRect(QRectF(0, 0, 0, 0));

    ui->graphicsView->setScene(scene);
    connect(scene, SIGNAL(processorSelected(int)), SIGNAL(processorSelected(int)));
}

ProcessorsWidget::~ProcessorsWidget()
{
    delete ui;
}

void ProcessorsWidget::saveCurrentState(QXmlStreamWriter &writer)
{
    QList<ProcessorItem*> data = scene->getItemMap().values();

    foreach(ProcessorItem* item, data)
    {
        QPointF p = item->pos();
        writer.writeStartElement("processor");
        writer.writeAttribute("name", item->getName());
        writer.writeAttribute("x", QString::number(p.x()));
        writer.writeAttribute("y", QString::number(p.y()));
        writer.writeEndElement();
    }
}

void ProcessorsWidget::restoreState(QXmlStreamReader &reader)
{
    scene->reset();
    reader.readNextStartElement();
    while(reader.name() == "processor")
    {
        QString name = reader.attributes().value("name").toString();
        QPoint p;
        p.setX(reader.attributes().value("x").toString().toInt());
        p.setY(reader.attributes().value("y").toString().toInt());
        scene->setPosition(name, p);
        reader.skipCurrentElement();
        if(!reader.readNextStartElement())
            break;
    }
    reader.skipCurrentElement();
}
