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


#include "ScaleBarOverlay.h"
#include "VideoDisplayWidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

ScaleBarOverlay::ScaleBarOverlay(const QString &name)
    : RectOverlay(name),
      mFont("Arial")
{
    mPen.setColor(QColor(Qt::lightGray));
    mPen.setWidth(3);

}

void ScaleBarOverlay::paintContent(QPainter &painter)
{
    double scale;
    {
        QMutexLocker lock(&mMutex);
        scale = mLastValue.toDouble();
    }
    if(mActive)
    {
        painter.setBrush(QColor(0,0,0, 160));
        painter.drawRect(mRect);
    }
    double scaleBarLen = mRect.width();
    double scaleValue = -1;
    if(scale > 0)
    {
        scaleValue = visibleScale(mRect.width() * scale);
        scaleBarLen = scaleValue /scale;
    }
    int barHeight = qMin(20, mRect.height()/2);
    QPointF p1(mRect.left(), mRect.bottom() - barHeight);
    QPointF p2(mRect.left()+scaleBarLen, mRect.bottom() - barHeight);
    painter.setPen(mPen);
    painter.drawLine(p1, p2);
    painter.drawLine(p1 + QPoint(0, barHeight), p1 - QPoint(0, barHeight));
    painter.drawLine(p2 + QPoint(0, barHeight), p2 - QPoint(0, barHeight));
    QRectF paintRect(p1 - QPoint(0,qMin(50, mRect.height()/2)), p2);
    mFont.setPixelSize(paintRect.height()*7/8);
    painter.setFont(mFont);
    QStringList unitNames = QStringList("m") <<  "mm" << "µm" << "nm";
    QString suffix = " "+ unitNames[0];
    for(int i=1; i<unitNames.size();i++)
    {
        if(scaleValue >= 1 || scaleValue < 0)
            break;
        scaleValue *= 1000;
        suffix = " "+unitNames[i];
    }
    painter.drawText(paintRect, Qt::AlignCenter, QString::number(scaleValue) + suffix);
}

double ScaleBarOverlay::visibleScale(double maxLen)
{
    int factor = 1;
    while(maxLen*factor > 10)
        factor /= 10;
    while(maxLen*factor<1)
        factor*=10;
    maxLen = (int)(maxLen*factor);
    return maxLen / factor;
}

void ScaleBarOverlay::mousePressEvent(QMouseEvent *event)
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

void ScaleBarOverlay::readElement(QXmlStreamReader &reader)
{
    if(reader.name() == "color")
        mPen.setColor(QColor(reader.readElementText().trimmed()));
    else
        RectOverlay::readElement(reader);
}

void ScaleBarOverlay::writeCurrentConfig(QXmlStreamWriter &writer)
{
    RectOverlay::writeCurrentConfig(writer);
    writer.writeTextElement("color", mPen.color().name());
}

void ScaleBarOverlay::setInitialPos(const QPoint &pos)
{
    mRect = QRect(pos, QSize(100,45));
}
