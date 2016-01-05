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

#include <ports/RectPort.h>
#include <QStringList>
#include <core/PortInfo.h>


QRectF port::Rect::qrect(const cv::Rect &r)
{
    return QRectF(r.x, r.y, r.width, r.height);
}

cv::Rect port::Rect::rect(const QRectF &r)
{
    return cv::Rect(r.x(), r.y(), r.width(), r.height());
}


port::Rect::Rect() : OverlayPort(QMetaType::QRectF, "Rect")
{
    setIcon(QImage(":/defaultPorts/images/rect.png"));
    setWidthConstraint(0);
    setHeightConstraint(0);
    setMin(QSizeF(1,1));
    mSquareOnly = false;
}


void port::Rect::setDefault(const QRectF& value)
{
    setDefaultValueRaw(value);
}

void port::Rect::setMax(const QRectF& value)
{
    mMax = value;
    setConstraint("max", value);
    recheck();
}

void port::Rect::setMin(const QSizeF& value)
{
    mMin = value;
    setConstraint("min", value);
    recheck();
}

QString port::Rect::toString(const QVariant& var) const
{
    QRectF r = var.toRectF();
    return QString("%1,%2,%3,%4").arg(r.x()).arg(r.y()).arg(r.width()).arg(r.height());
}

QVariant port::Rect::fromString(const QString& str) const
{
    QStringList p = str.split(",");
    if(p.length() < 4)
        return QRectF();
    return QRectF(p[0].toDouble(), p[1].toDouble(), p[2].toDouble(), p[3].toDouble());
}

QVariant port::Rect::constrainedValue(const QVariant &var) const
{
    QRectF r = var.toRectF();
    r = constrainDimensions(r);
    if(mMax.isValid())
    {
        if(!mMax.contains(r))
        {

            r = r.intersected(mMax);
            // the intersect might break the before applied corrections
            r = constrainDimensions(r);
            // check if all is right now ...
            if(!mMax.contains(r))
            {
                // the rect is most likely out of the max rect
                r.moveBottomRight(mMax.bottomRight());
            }
        }
    }

    return QVariant(r);
}

QRectF port::Rect::constrainDimensions(QRectF r) const
{

    if(mWDiv)
    {
        while(((int)r.width() % mWDiv) != mWRes)
            r.setWidth(r.width() + mWStep);
    }
    if(mHDiv)
    {
        while(((int)r.height() % mHDiv) != mHRes)
            r.setHeight(r.height() + mHStep);
    }
    if(mMin.isValid())
    {
        if(r.width() < mMin.width())
            r.setWidth(mMin.width());
        if(r.height() < mMin.height())
            r.setHeight(mMin.height());
    }
    if(mSquareOnly)
    {
        if(r.width() != r.height())
        {
            double val = qMin(r.width(), r.height());
            r.setWidth(val);
            r.setHeight(val);
        }
    }
    return r;
}

void port::Rect::setWidthConstraint(int divisor, int result, bool roundDown)
{
    mWDiv = divisor;
    mWRes = result;
    if(roundDown)
        mWStep = -1;
    else
        mWStep = +1;
}

void port::Rect::setHeightConstraint(int divisor, int result, bool roundDown)
{
    mHDiv = divisor;
    mHRes = result;
    if(roundDown)
        mHStep = -1;
    else
        mHStep = +1;
}

void port::Rect::setSquareOnly(bool onlySquareShapeAllowed)
{
    mSquareOnly = onlySquareShapeAllowed;
    setConstraint("squareShapeOnly", onlySquareShapeAllowed);
}

cv::Rect in::Rect::getValue()
{
    return rect(getRawValue().toRectF());
}

QRectF in::Rect::getValue2()
{
    return getRawValue().toRectF();
}


void out::Rect::send(const QRectF& value)
{
    sendRaw(value);
}

