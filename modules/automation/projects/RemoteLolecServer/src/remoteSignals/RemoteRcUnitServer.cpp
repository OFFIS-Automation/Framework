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


#include "RemoteRcUnitServer.h"
#include "../RcUnits.h"

#include <QDebug>

RemoteRcUnitServer::RemoteRcUnitServer(RcUnits* rcUnits, QIODevice *device, bool doInitialize)
    : RemoteRcUnitServerBase(device, device, false)
{
    mRcUnits = rcUnits;
    connect(this, SIGNAL(listUnits()), this, SLOT(onListUnits()));
    connect(this, SIGNAL(callMethod(uint,QByteArray,QByteArray,QVariantList)), rcUnits, SLOT(callAsync(uint,QByteArray,QByteArray,QVariantList)));
    connect(rcUnits, SIGNAL(asyncError(uint,QString)), SLOT(methodError(uint,QString)));
    connect(rcUnits, SIGNAL(asyncResponse(uint,QVariant)), SLOT(methodResponse(uint,QVariant)));
    if(doInitialize)
        initialize();
}

void RemoteRcUnitServer::onListUnits()
{
    qDebug() << "list units";
    QList<RcUnitHelp> list = mRcUnits->getHelpList().values();
    unitList(list);
}

