// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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

#include "DataOverlay.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QColorDialog>
#include <QMouseEvent>
#include <QDebug>

QList<QColor> DataOverlay::sOverlayColors;

DataOverlay::DataOverlay(QString name)
    : RectOverlay(name), mPen(Qt::darkRed), mFont("Arial")
{
    mPen.setWidth(2);
    mPen.setCosmetic(true);
    mPen.setJoinStyle(Qt::MiterJoin);
    if(sOverlayColors.empty()) // initialize if empty
    {
        sOverlayColors << Qt::red;
        sOverlayColors << Qt::blue;
        sOverlayColors << Qt::green;
        sOverlayColors << Qt::cyan;
        sOverlayColors << Qt::magenta;
        sOverlayColors << Qt::yellow;
        sOverlayColors << Qt::darkRed;
        sOverlayColors << Qt::darkBlue;
        sOverlayColors << Qt::darkGreen;
        sOverlayColors << Qt::darkCyan;
        sOverlayColors << Qt::darkMagenta;
        sOverlayColors << Qt::darkYellow;
    }
    mPen.setColor(sOverlayColors.takeFirst());

}

void DataOverlay::writeCurrentConfig(QXmlStreamWriter &writer)
{
    RectOverlay::writeCurrentConfig(writer);
    writer.writeStartElement("color");
    writer.writeCharacters(mPen.color().name());
    writer.writeEndElement();
}

void DataOverlay::readElement(QXmlStreamReader &reader)
{
    if (reader.name() == "color") {
        QString colorString = reader.readElementText();
        mPen.setColor(QColor(colorString));
    } else {
        RectOverlay::readElement(reader);
    }
}

void DataOverlay::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        event->accept();
        QColor color = QColorDialog::getColor(mPen.color(), mWidget, tr("Select new overlay color"));
        if(color.isValid())
            mPen.setColor(color);
    } else {
        RectOverlay::mousePressEvent(event);
    }
}

void DataOverlay::paintContent(QPainter &painter)
{
    painter.fillRect(mRect, QColor(0, 0, 0, 160));
    painter.setPen(mPen);

    painter.setViewTransformEnabled(false);
    QRect textRect = mTransform.mapRect(mRect);
    textRect.setLeft(textRect.left() + textRect.height());
    mFont.setPixelSize(textRect.height() * 7/8);
    painter.setFont(mFont);
    painter.drawText(textRect, Qt::AlignVCenter, legendString());
    QRect symbolRect = mTransform.mapRect(mRect);
    symbolRect.setRight(symbolRect.left() + symbolRect.height());
    painter.setViewTransformEnabled(true);
    paintLegendSymbol(painter, mTransform.inverted().mapRect(symbolRect));
}

void DataOverlay::paintLegendSymbol(QPainter &painter, const QRect &rect)
{
    painter.setPen(mPen);
    painter.drawPoint(rect.center());
}

QString DataOverlay::legendString()
{
    return mVisionInterface->getFilter(mPortId.filter).name + "." + mVisionInterface->getPortInfo(mPortId).name;
}
