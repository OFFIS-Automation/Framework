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

#ifndef RECTPORTOVERLAY_H
#define RECTPORTOVERLAY_H

#include "RectOverlay.h"

class RectPortOverlay : public RectOverlay
{
public:
    RectPortOverlay(QString name);

    virtual void setInitialPos(const QPoint &pos);
    virtual bool isReadOnly() { return isOutput(); }
    virtual void mouseMoveEvent(QMouseEvent* event, QList<QPoint> snapPoints);
    virtual void mouseReleaseEvent(QMouseEvent* event);

    virtual void setValue(const QVariant &value);
    //virtual void paint(QPainter& painter, bool showControls);
    virtual void ensureBounds();
};

#endif // RECTPORTOVERLAY_H
