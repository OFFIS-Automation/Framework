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

#ifndef SIMPLESHAPEOVERLAY_H
#define SIMPLESHAPEOVERLAY_H

#include "DataOverlay.h"

class SimpleShapeOverlay : public DataOverlay
{
public:
    SimpleShapeOverlay(QString name);
    virtual void paint(QPainter& painter, bool showControls);
    virtual void paintElement(QPainter& painter, const QVariant& element);
    virtual void paintLegendSymbol(QPainter& painter, const QRect& rect);

    virtual void setPortId(const PortId &portId, bool output);

    virtual void mousePressEvent(QMouseEvent* event);
    virtual bool contains(const QPoint &pos);

protected:
    QString mPortTypeName;
    QPointF mPoint;
};

#endif // SIMPLESHAPEOVERLAY_H
