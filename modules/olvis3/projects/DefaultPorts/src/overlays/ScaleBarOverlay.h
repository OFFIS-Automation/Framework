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


#ifndef SCALEBAROVERLAY_H
#define SCALEBAROVERLAY_H

#include "RectOverlay.h"

class ScaleBarOverlay : public RectOverlay
{
public:
    ScaleBarOverlay(const QString& name);
    void paintContent(QPainter &painter);
    void mousePressEvent(QMouseEvent *event);
    void readElement(QXmlStreamReader &reader);
    void writeCurrentConfig(QXmlStreamWriter &writer);
    void setInitialPos(const QPoint &pos);
protected:
    double visibleScale(double maxLen);
    double mScale;
    QFont mFont;
    QPen mPen;
};

#endif // SCALEBAROVERLAY_H
