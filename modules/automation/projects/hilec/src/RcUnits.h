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

#ifndef RCUNITS_H
#define RCUNITS_H

#include <QString>
#include <QMap>
#include <QVariant>

#include <RcUnitsBase.h>
#include <RcUnitBase.h>

class QXmlStreamReader;
class RcUnitInterface;
class RcUnit;
class Gamepad;
class RemoteRcUnits;



class RcUnits : public RcUnitsBase, public RcUnitBaseObserver
{
    Q_OBJECT
public:
    RcUnits(const QString& rcUnitDir);
    virtual ~RcUnits();
    static RcUnits* instance() { return mInstance; }
    void loadConfig(const QString &filename);
    void releaseConfig();
    void rcUnitStatusChanged(bool acquired);
signals:
    void unitListUpdated(bool partialUpdate = false);
private slots:
    void onRemoteRcUnitsListed(const QString& remoteServerName, const QStringList& oldRcUnits);
private:
    static RcUnits* mInstance;
    QMap<QString, RemoteRcUnits*> mRemoteRcUnits;
};

#endif // RCUNITS_H
