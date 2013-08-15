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


#ifndef GRAPHOVERLAY_H
#define GRAPHOVERLAY_H

#include "RectOverlay.h"

class GraphOverlay : public RectOverlay
{
    Q_OBJECT
public:
    explicit GraphOverlay(const QString& name);
    void paintContent(QPainter &p);
    void setInitialPos(const QPoint &pos);
protected:
    bool mShowAsGraph;
    void mousePressEvent(QMouseEvent *event);
    QList<double> mValues;
    double mMin, mMax;
    QPolygonF mCurve;
    double mCurrentValue;
    QFont mFont;
};

#endif // GRAPHOVERLAY_H
