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

#include "StringOverlay.h"

#include <QPainter>
#include <QDebug>
#include <QInputDialog>
#include <QMouseEvent>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

StringOverlay::StringOverlay(QString name)
    : RectOverlay(name),
        mString(""),
        mFormatString("%1: %2"),
        mFont("Arial")
{
}

void StringOverlay::paintContent(QPainter &p)
{
    mMutex.lock();
    if (mLastValue.isValid()) {
        mString = format(mLastValue);
        mLastValue = QVariant();
    }
    mMutex.unlock();
    p.setBrush(QColor(0, 0, 0, 160));
    mPen.setColor(Qt::white);
    p.setPen(mPen);
    p.drawRoundedRect(mRect, 5.0, 5.0);
    p.setBrush(Qt::NoBrush);

    p.setViewTransformEnabled(false);
    QRect textRect = mTransform.mapRect(mRect);
    textRect.setLeft(textRect.left() + 2);
    QRect realRect = mTransform.mapRect(mRect);
    mFont.setPixelSize(realRect.height() * 7/8);
    p.setFont(mFont);
    p.drawText(textRect, Qt::AlignLeft, mString);
    p.setViewTransformEnabled(true);
}

void StringOverlay::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        event->accept();
        mFormatString = QInputDialog::getText(mWidget, tr("Enter overlay text"), tr("overlay text"), QLineEdit::Normal, mFormatString);
        mString = format(mVisionInterface->getPortValue(mPortId));
    } else {
        RectOverlay::mousePressEvent(event);
    }
}

void StringOverlay::setPortId(const PortId &portId, bool output)
{
    RectOverlay::setPortId(portId, output);
    PortInfo info = mVisionInterface->getPortInfo(portId);
    QVariantList values = info.constraints.value("choices").toList();
    QStringList names = info.constraints.value("choiceNames").toStringList();
    for(int i=0;i<values.size(); i++)
    {
        QVariant value = values[i];
        QString valAsString = mVisionInterface->portValueString(portId, value);
        QString name = names.value(i, valAsString);
        mPreDefinedStrings.insert(valAsString, name);
    }
}

QString StringOverlay::format(QVariant value)
{
    QString str = mFormatString;
    QString val = mVisionInterface->portValueString(mPortId, value);
    val = mPreDefinedStrings.value(val, val);
    if (str.contains("%1"))
        str = str.arg(mPortId.port);
    if (str.contains("%2"))
        str = str.arg(val);
    if (str.contains("%3"))
    {
        QStringList list = val.split("/", QString::SkipEmptyParts);
        if(!list.isEmpty())
            str = str.arg(list.last());
        else
            str = str.arg(val);
    }
    return str;
}

void StringOverlay::writeCurrentConfig(QXmlStreamWriter &writer)
{
    RectOverlay::writeCurrentConfig(writer);
    writer.writeTextElement("stringFormat", mFormatString);
}

void StringOverlay::readElement(QXmlStreamReader &reader)
{
    if(reader.name() == "stringFormat")
        mFormatString = reader.readElementText().trimmed();
    else
        RectOverlay::readElement(reader);
}
