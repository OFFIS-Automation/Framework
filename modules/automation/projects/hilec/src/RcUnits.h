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

#ifndef RCUNITS_H
#define RCUNITS_H

#include <QString>
#include <QMap>
#include <QVariant>
#include <QThreadPool>

#include <RcUnitsBase.h>
#include <RcUnitBase.h>
#include <QTimer>


class QXmlStreamReader;
class RcUnitInterface;
class RcUnit;
class RemoteRcUnits;
class FlagCollectorRunnable;


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
    void flagsUpdated(const QString& name, const QVariantList& values);
private slots:
    void collectFlags();
#ifndef NO_REMOTE_CLIENT
    void onRemoteRcUnitsListed(const QString& remoteServerName, const QStringList& oldRcUnits);
#endif
private:
    QTimer mFlagTimer;
    static RcUnits* mInstance;
    QList<FlagCollectorRunnable*> mFlagCollectors;
    QThreadPool mThreadPool;
#ifndef NO_REMOTE_CLIENT
    QMap<QString, RemoteRcUnits*> mRemoteRcUnits;
#endif
};

#endif // RCUNITS_H
