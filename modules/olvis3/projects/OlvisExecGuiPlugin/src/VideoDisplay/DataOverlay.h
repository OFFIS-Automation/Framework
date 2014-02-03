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

#ifndef DATAOVERLAY_H
#define DATAOVERLAY_H

#include "RectOverlay.h"

#include <QPen>

class DataOverlay : public RectOverlay
{
    Q_OBJECT;
public:
    DataOverlay(QString name);

    virtual void writeCurrentConfig(QXmlStreamWriter& writer);
    virtual void readElement(QXmlStreamReader& reader);

    virtual void mousePressEvent(QMouseEvent *event);

    virtual void paintLegendSymbol(QPainter& painter, const QRect& rect);
    virtual QString legendString();

    virtual void paintContent(QPainter& painter);

protected:
    QPen mPen;
    QFont mFont;
    static QList<QColor> sOverlayColors;
};

#endif // DATAOVERLAY_H
