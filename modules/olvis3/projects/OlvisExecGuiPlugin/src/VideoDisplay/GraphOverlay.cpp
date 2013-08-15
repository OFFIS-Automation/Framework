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


#include "GraphOverlay.h"

GraphOverlay::GraphOverlay(const QString &name) :
    RectOverlay(name), mFont("Arial")
{
    mMin = DBL_MAX;
    mMax = DBL_MIN;
}


void GraphOverlay::paintContent(QPainter &p)
{
    QVariant value;
    {
        QMutexLocker lock(&mMutex);
        value = mLastValue;
        mLastValue = QVariant();
    }
    if(value.canConvert(QVariant::Double))
    {
        int border = 2;
        QRectF rect(mRect.topLeft() + QPoint(border, border), mRect.bottomRight()- QPoint(border, border));
        double val = value.toDouble();
        if(val < mMin)
            mMin = val;
        if(val > mMax)
            mMax = val;
        mValues << val;
        mCurrentValue = val;
        double xStep = (double)rect.size().width() / qMax(10, mValues.size());
        double yFactor = -(rect.size().height()/(mMax-mMin));
        QPolygonF poly;
        double pos = 0;
        foreach(double val, mValues)
        {
            poly << rect.bottomLeft() + QPointF(pos, (val-mMin)*yFactor);
            pos += xStep;
        }
        mCurve = poly;
    }
    QColor c = Qt::red;
    p.setPen(c);
    p.drawPolyline(mCurve);
    p.setPen(QColor(Qt::green));
    QRectF textRect(mRect.bottomLeft() - QPoint(-5, 20), mRect.bottomRight());
    mFont.setPixelSize(textRect.height() * 6/8);
    p.setFont(mFont);
    p.drawText(textRect, QString::number(mMin));
    textRect = QRect(mRect.topLeft() + QPoint(5, 0), mRect.topRight() + QPoint(0, 20));
    p.drawText(textRect, QString::number(mMax));

    textRect = QRect(mRect.bottomLeft() - QPoint(-5, 30), mRect.bottomRight() - QPoint(5,0));
    mFont.setPixelSize(textRect.height() * 6/8);
    p.setFont(mFont);
    p.drawText(textRect, Qt::AlignRight, QString::number(mCurrentValue));
    p.setPen(QColor(Qt::black));
    if (mCurve.isEmpty())
        p.setBrush(QColor(0, 0, 0, 128));
    else
        p.setBrush(QColor(255, 255, 255, 80));
    p.drawRect(mRect);
}

void GraphOverlay::setInitialPos(const QPoint &pos)
{
       mRect.setRect(pos.x(), pos.y(), 200, 150);
}

void GraphOverlay::mousePressEvent(QMouseEvent *event)
{
    RectOverlay::mousePressEvent(event);
}

