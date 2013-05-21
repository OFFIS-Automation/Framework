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

#include "RcUnits.h"
#include <QCoreApplication>
#include <QDir>
#include <RcUnit.h>

RcUnits* RcUnits::sInst = 0;



class UnitInvokerRunnable : public QRunnable
{
public:
    UnitInvokerRunnable(RcUnits& units, const QByteArray& unit, const QByteArray& method, const QList<QVariant>& params, int id) :
        mUnits(units),
        mUnit(unit),
        mMethod(method),
        mParams(params),
        mId(id) {}

    void run()
    {
        try
        {
            QVariant retVal = mUnits.call(mUnit, mMethod, mParams);
            mUnits.sendAsyncResponse(mId, retVal);
        }
        catch(const std::exception& e) {
            mUnits.sendAsyncError(mId, QString(e.what()));
        } catch(...) {
            mUnits.sendAsyncError(mId, QObject::tr("unknown error"));
        }
    }
protected:
    RcUnits& mUnits;
    const QByteArray mUnit, mMethod;
    const QList<QVariant> mParams;
    int mId;
};

RcUnits::RcUnits()
{
    sInst = this;
    mLolecDir = QDir(QCoreApplication::applicationDirPath() + "/plugins/lolecs").absolutePath();
}

RcUnits::~RcUnits()
{
    sInst = 0;
}

RcUnits * RcUnits::instance()
{
    return sInst;
}

QMap<QString, RcUnitHelp> RcUnits::getHelpList()
{
    QMap<QString, RcUnitHelp> list;
    foreach(QString name, mUnits.keys())
        list[name] = getHelp(name);
    return list;
}

void RcUnits::callAsync(const QByteArray &unit, const QByteArray &name, int returnId, const QList<QVariant>& params)
{
    mThreads.start(new UnitInvokerRunnable(*this, unit, name, params, returnId));
}

void RcUnits::sendAsyncResponse(int id, const QVariant &var)
{
    emit asyncResponse(id, var);
}

void RcUnits::sendAsyncError(int id, const QString &error)
{
    emit asyncError(id, error);
}

void RcUnits::updateSensitivity(const QString &unitName, const QString &tcName, double sens, const QList<bool> &inverts)
{
    RcUnitBase* unit = mUnits.value(unitName, 0);
    if(!unit)
        return;
    unit->updateSensitivity(tcName, sens, inverts);
}

void RcUnits::handleTcData(const QMap<int, double>& data)
{
    mGamepad.handleData(data);
}

void RcUnits::handleTcButton(int button, bool pressed)
{
    mGamepad.handleButton(button, pressed);
}

void RcUnits::enableGamepad(const QString &unitName)
{
    RcUnitBase* unit = mUnits.value(unitName, 0);
    if(!unit)
        return;
    unit->connectGamepad(&mGamepad);
}

void RcUnits::disableGamepad(const QString &unitName)
{
    RcUnitBase* unit = mUnits.value(unitName, 0);
    if(!unit)
        return;
    unit->disconnectGamepad(&mGamepad);
}
