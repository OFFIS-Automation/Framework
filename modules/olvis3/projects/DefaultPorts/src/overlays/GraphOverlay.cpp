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


#include "GraphOverlay.h"

#include "GraphOverlayOptions.h"

#include <QMouseEvent>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

GraphOverlay::GraphOverlay(const QString &name) :
    RectOverlay(name)
{
    onClearClicked();
    mShowHistory = false;
    mLimitedValues = true;
    mMaxValues = 100;
    mListId = 0;
    connect(this, SIGNAL(clearClicked()), SLOT(onClearClicked()));
}

void GraphOverlay::writeCurrentConfig(QXmlStreamWriter &writer)
{
    RectOverlay::writeCurrentConfig(writer);
    writer.writeTextElement("showHistory", mShowHistory ? "1" : "0");
    writer.writeTextElement("limitedValuesOnly", mLimitedValues ? "1" : "0");
    writer.writeTextElement("limitedValuesMax", QString::number(mMaxValues));
    writer.writeTextElement("valueListId", QString::number(mListId));
}

void GraphOverlay::readElement(QXmlStreamReader &reader)
{
    if(reader.name() == "showHistory")
        mShowHistory = reader.readElementText().trimmed() == "1";
    else if(reader.name() == "limitedValuesOnly")
        mLimitedValues = reader.readElementText().trimmed() == "1";
    else if(reader.name() == "limitedValuesMax")
        mMaxValues = qMax(10, reader.readElementText().trimmed().toInt());
    else if(reader.name() == "valueListId")
        mListId = reader.readElementText().trimmed().toInt();
    else
        RectOverlay::readElement(reader);
}

void GraphOverlay::paintContent(QPainter &p)
{
    QVariant value;
    {
        QMutexLocker lock(&mMutex);
        value = mLastValue;
        mLastValue = QVariant();
        if(value.type() == QVariant::List)
            value = value.toList().value(mListId, QVariant());
        if(value.canConvert(QVariant::Double))
            mCurrentValue = value.toDouble();
    }
    if(mShowHistory)
    {
        if(value.canConvert(QVariant::Double))
        {
            double val = value.toDouble();
            mCurrentValue = val;
            int border = 2;
            QRectF rect(mRect.topLeft() + QPoint(border, border), mRect.bottomRight()- QPoint(border, border));
            if(val < mMin)
                mMin = val;
            if(val > mMax)
                mMax = val;
            if(mMax == mMin)
                mMax = mMin + 0.00000001;
            mValues << val;
            while(mLimitedValues && mValues.size() > mMaxValues)
                mValues.pop_front();

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
    }
    p.setPen(QColor(Qt::black));
    p.setBrush(QColor(0, 0, 0, 160));
    p.drawRect(mRect);
    if(mShowHistory)
    {
        QColor c = QColor(255, 0, 0);
        p.setPen(c);
        p.drawPolyline(mCurve);
        p.setPen(QColor(Qt::green));
        QRectF textRect(mRect.bottomLeft() - QPoint(-5, 20), mRect.bottomRight());
        mFont.setPixelSize(textRect.height() * 6/8);
        p.setFont(mFont);
        p.drawText(textRect, QString::number(mMin));
        textRect = QRect(mRect.topLeft() + QPoint(5, 0), mRect.topRight() + QPoint(0, 20));
        p.drawText(textRect, QString::number(mMax));
    }
    p.setPen(QColor(Qt::green));
    QRect textRect = QRect(mRect.bottomLeft() - QPoint(-5, 30), mRect.bottomRight() - QPoint(5,0));
    mFont.setPixelSize(textRect.height() * 6/8);
    p.setFont(mFont);
    p.drawText(textRect, Qt::AlignRight, QString::number(mCurrentValue));
}

void GraphOverlay::setInitialPos(const QPoint &pos)
{
    mRect.setRect(pos.x(), pos.y(), 200, 150);
}

void GraphOverlay::onClearClicked()
{
    mValues.clear();
    mMin = DBL_MAX;
    mMax = DBL_MIN;
}

void GraphOverlay::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        event->accept();
        GraphOverlayOptions dialog(mShowHistory, mLimitedValues, mMaxValues, mListId, mWidget);
        if(dialog.exec())
        {
            mShowHistory = dialog.showHistory();
            if(!mShowHistory)
                onClearClicked();
            mLimitedValues = dialog.limitedValues();
            mMaxValues = dialog.maxValues();
            if(mListId != dialog.listId())
                onClearClicked();
            mListId = dialog.listId();
        }
    } else {
        RectOverlay::mousePressEvent(event);
    }
}

