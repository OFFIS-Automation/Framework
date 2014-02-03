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

#include "RcUnits.h"
#include "RcUnit.h"

#include <QtEndian>
#include <QDebug>
#include <QCoreApplication>
#include <QFileInfo>
#include <QSettings>
#include <QDir>
#include <QPluginLoader>
#include <lolecs/LolecInterface.h>
#include <QTcpSocket>

#include "RemoteLolec.h"
#include "RemoteRcUnit.h"


RcUnits* RcUnits::mInstance = 0;

RcUnits::RcUnits(const QString &lolecDir)
{
    if(mInstance)
        throw std::runtime_error("RcUnits already running");
    mInstance = this;
    mLolecDir = lolecDir;
    mGamepad = 0;
    connect(this, SIGNAL(unitsUpdated()), SIGNAL(unitListUpdated()));
}

RcUnits::~RcUnits()
{
}

void RcUnits::loadConfig(const QString &filename)
{
    mConfigFile = filename;
    RcUnitsBase::loadConfig(filename);
    foreach(QString key, mUnits.keys())
    {
        RcUnitBase* unit = mUnits.value(key);
        if(unit->isTelecontrolable())
            mTelecontrolLolecs << key;
    }

    QSettings settings(filename, QSettings::IniFormat);
    int size = settings.beginReadArray("remoteServers");
    for(int i=0; i< size; i++)
    {
        settings.setArrayIndex(i);
        QString name = settings.value("name").toString();
        QString host = settings.value("address").toString();
        int port = settings.value("port").toInt();
        double timeout = settings.value("callTimeout").toDouble();
        RemoteLolec* rl = new RemoteLolec(name, host, port);
        rl->setTimeout(timeout);
        connect(rl, SIGNAL(lolecsUpdated(QString, QStringList)), SLOT(onRemoteLolecsListed(QString, QStringList)));
        mRemoteLolecs[name] = rl;
        rl->startConnect();
    }
}

void RcUnits::onRemoteLolecsListed(const QString &remoteServerName, const QStringList &oldLolecs)
{
    RemoteLolec* rl = mRemoteLolecs.value(remoteServerName, 0);
    if(!rl)
        return;
    QList<RcUnitBase*> units = rl->units();
    QStringList lolecsToRemove = oldLolecs;
    foreach(RcUnitBase* unit, units)
        lolecsToRemove << unit->name();
    foreach(QString name, lolecsToRemove)
    {
        RcUnitBase* old = mUnits.value(name, 0);
        if(old)
        {
            // if this is a local unit, dont delete, dont overwrite
            if(dynamic_cast<RcUnit*>(old))
                continue;
            // else, delete
            mUnits.take(name);
            delete old;
        }
    }
    foreach(RcUnitBase* unit, units)
    {
        if(mUnits.contains(unit->name())) // local unit with same name is here
            delete unit;
        else
            mUnits[unit->name()] = unit;
    }

    emit unitListUpdated(true);
}

