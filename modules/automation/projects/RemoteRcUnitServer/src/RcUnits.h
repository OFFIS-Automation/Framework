// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
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

#include <RcUnitsBase.h>
#include <QThreadPool>
#include "GamepadWrapper.h"

class RcUnits : public RcUnitsBase
{
    Q_OBJECT
public:
    explicit RcUnits();
    virtual ~RcUnits();
    static RcUnits* instance();
    QMap<QString, RcUnitHelp> getHelpList();
public slots:
    void callAsync(uint id, const QByteArray &unit, const QByteArray &name, const QVariantList& params);
    void updateSensitivity(const QString& unit, const QString& tcName, double sens, const QList<bool>& inverts);
    void sendAsyncResponse(int id, const QVariant& var);
    void sendAsyncError(int id, const QString& error);
    void handleTcData(const QMap<int, double> &data);
    void handleTcButton(int button ,bool pressed);
    void enableGamepad(const QString& unit);
    void disableGamepad(const QString& unit);
signals:
    void asyncResponse(uint id, const QVariant& var);
    void asyncError(uint id, const QString& var);
private:
    GamepadWrapper mGamepad;
    void setGamepad(const QString& unit, QObject* gamepad);
    static RcUnits* sInst;
    QThreadPool mThreads;
};

#endif // RCUNITS_H
