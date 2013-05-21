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

#ifndef OVERLAYFACTORY_H
#define OVERLAYFACTORY_H

#include <QString>
#include <core/PortId.h>
#include <QSet>
#include <QObject>

class Overlay;

class OverlayFactory : public QObject
{
public:
    static OverlayFactory& instance();

    Overlay* createOverlay(const QString& name);
    Overlay* createOverlay(const PortId& portId, bool output, bool main);

private:
    QSet<QString> mStringOverlays;
    OverlayFactory();
    Q_DISABLE_COPY(OverlayFactory);
};


#endif // OVERLAYFACTORY_H
