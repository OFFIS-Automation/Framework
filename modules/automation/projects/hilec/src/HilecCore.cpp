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

#include "HilecCore.h"
#include "RcUnits.h"
#include "telecontrol/TelecontrolFactory.h"

#include <QList>
#include <QDebug>
#include <QStringList>
#include <QFileInfo>
#include <stdexcept>

HilecCore* HilecCore::mInstance = 0;

HilecCore::HilecCore(const QString &configDir) : mPython(configDir)
{
    if(mInstance){
        throw std::runtime_error(tr("HilecCore already running").toStdString());
    }
    mInstance = this;
    qRegisterMetaType< QList<bool> >("QList<bool>");
    qRegisterMetaType<ScriptCompileInfo>("ScriptCompileInfo");
    connect(&mRequests, SIGNAL(newUserRequest(UserRequest)), SIGNAL(userInputRequested(UserRequest)));
    connect(&mRequests, SIGNAL(requestAborted(int)), SIGNAL(userInputAborted(int)));
    connect(&mPython, SIGNAL(finished()), SIGNAL(scriptExecutionFinished()));
    connect(&mPython, SIGNAL(started()), SIGNAL(scriptExecutionStarted()));
    connect(RcUnits::instance(), SIGNAL(unitListUpdated(bool)), SIGNAL(rcUnitsChanged(bool)));

    connect(RcUnits::instance(), SIGNAL(telecontrolAssignmentUpdated(QString,QString)), SIGNAL(telecontrolAssignmentUpdated(QString,QString)));
    connect(RcUnits::instance(), SIGNAL(gamepadUpdated(QString, QString, bool)), SIGNAL(gamepadUpdated(QString, QString, bool)));
    connect(RcUnits::instance(), SIGNAL(gamepadSensitivityChangeRequested(QString, QString, bool)), SIGNAL(gamepadSensitivityChangeRequested(QString, QString, bool)));
    connect(RcUnits::instance(), SIGNAL(gamepadSwitchRequested(QString, QString, bool)), SIGNAL(gamepadSwitchRequested(QString, QString, bool)));
    connect(RcUnits::instance(), SIGNAL(hapticUpdated(QString, QString, bool)), SIGNAL(hapticUpdated(QString, QString, bool)));
    connect(RcUnits::instance(), SIGNAL(flagsUpdated(QString,QVariantList)), SIGNAL(rcUnitFlagsUpdated(QString,QVariantList)));
}

HilecCore::~HilecCore()
{
    waitForStop();
}

bool HilecCore::waitForStop(uint timeout)
{
    mPython.quit();
    return mPython.wait(timeout);
}

void HilecCore::runFile(const QString &filename)
{
    if(mPython.isRunning()){
        resume();
    } else {
        mPython.start(filename, mBaseDir);
    }
}

void HilecCore::compileFile(const QString &filename)
{
    mLint.compile(filename);
}

RcUnitHelp HilecCore::getUnitHelp(const QString &name)
{
    return RcUnits::instance()->getHelp(name);
}

QStringList HilecCore::getTelecontrolableUnits()
{
    return RcUnits::instance()->telecontrolableUnitNames();
}

QMap<QString, Gamepad *> HilecCore::getGamepadDevices()
{
    return TelecontrolFactory::getGamepadDevices();
}

QMap<QString, HapticDevice *> HilecCore::getHapticDevices()
{
    return TelecontrolFactory::getHapticDevices();
}

TelecontrolConfig HilecCore::getTelecontrolConfig(const QString &name)
{
    return RcUnits::instance()->getTelecontrolConfig(name);
}

QStringList HilecCore::rcUnits()
{
    return RcUnits::instance()->unitNames();
}

void HilecCore::loadFromFile(const QString &filename)
{
    setBaseDir(QFileInfo(filename).absolutePath());
    RcUnits::instance()->loadConfig(filename);
}

void HilecCore::releaseConfig()
{
    RcUnits::instance()->releaseConfig();
}

QWidget* HilecCore::createRcUnitWidget(const QString &rcUnit)
{
    return RcUnits::instance()->settingsWidget(rcUnit);
}

void HilecCore::userInput(int uid, int buttonId, const QList<QVariant> &data)
{
    mRequests.answer(uid, buttonId, data);
}

void HilecCore::addBreakpoint(QString file, int line)
{
    mPython.addBreakpoint(file, line);
}

void HilecCore::removeBreakpoint(QString file, int line)
{
    mPython.removeBreakpoint(file, line);
}

QList<QPair<QString, int> > HilecCore::breakpoints() const
{
    return mPython.breakpoints();
}

QList<int> HilecCore::breakpoints(const QString &filename) const
{
    typedef QPair<QString, int> BreakpointPair;
    QList<int> lines;
    foreach(BreakpointPair breakpoint, breakpoints())
    {
        if(QFileInfo(breakpoint.first) == QFileInfo(filename))
            lines << breakpoint.second;
    }
    return lines;
}

void HilecCore::resume()
{
    mPython.resume();
}

void HilecCore::pause()
{
    mPython.stepInto();
}

void HilecCore::quit()
{
    RcUnits::instance()->stopAll();
    mPython.quit();
}

void HilecCore::stepOver()
{
    mPython.stepOver();
}

void HilecCore::stepInto()
{
    mPython.stepInto();
}

void HilecCore::stepReturn()
{
    mPython.stepReturn();
}

QAbstractItemModel* HilecCore::getDebugVars(int frameDepth)
{
    return mPython.debugVars(frameDepth);
}

QList<TraceLine> HilecCore::getStackTrace()
{
    return mPython.getStackTrace();
}

void HilecCore::setBaseDir(const QString &baseDir)
{
    mBaseDir = baseDir;
    mLint.setBasePath(mBaseDir);
}

void HilecCore::updateTelecontrolAssignment(const QString& deviceName, const QString& unitName)
{
    RcUnits::instance()->updateTelecontrolAssignment(deviceName, unitName);
}

void HilecCore::activateGamepad(const QString& deviceName, const QString &unitName)
{
    RcUnits::instance()->activateGamepad(deviceName, unitName);
}

void HilecCore::deactivateGamepad(const QString &deviceName, const QString &unitName)
{
    RcUnits::instance()->deactivateGamepad(deviceName, unitName);
}

void HilecCore::deactivateGamepadAll()
{
    RcUnits::instance()->deactivateGamepadAll();
}

void HilecCore::updateGamepadParameters(const QString &unitName, const QString &methodName, double sensitivity, const QList<bool>& inverts)
{
    RcUnits::instance()->updateGamepadParameters(unitName, methodName, sensitivity, inverts);
}

void HilecCore::remoteGamepadDataUpdated(const QMap<int, double> &data, const QString &gamepadName)
{
   RcUnits::instance()->remoteGamepadDataUpdated(data, gamepadName);
}

void HilecCore::remoteGamepadButtonToggled(int buttonId, const bool &pressed, const QString &gamepadName)
{
    RcUnits::instance()->remoteGamepadButtonToggled(buttonId, pressed, gamepadName);
}

void HilecCore::activateHaptic(const QString& deviceName, const QString &unitName)
{
    RcUnits::instance()->activateHaptic(deviceName, unitName);
}

void HilecCore::deactivateHaptic(const QString& deviceName, const QString &unitName)
{
    RcUnits::instance()->deactivateHaptic(deviceName, unitName);
}

void HilecCore::deactivateHapticAll()
{
    RcUnits::instance()->deactivateHapticAll();
}

void HilecCore::updateHapticParameters(const QString &unitName, const QString &methodName, double sensitivity, double forceScaling, const QList<bool>& inverts)
{
    RcUnits::instance()->updateHapticParameters(unitName, methodName, sensitivity, forceScaling, inverts);
}

void HilecCore::callRcUnitAcquire(const QString &unitName)
{
    RcUnits::instance()->acquire(unitName);
}

void HilecCore::callRcUnitAcquireAll()
{
    RcUnits::instance()->acquireAll();
}

void HilecCore::callRcUnitRelease(const QString &unitName)
{
    RcUnits::instance()->release(unitName);
}

void HilecCore::callRcUnitReleaseAll()
{
    RcUnits::instance()->releaseAll();
}

void HilecCore::callRcUnitStop(const QString &unitName)
{
    RcUnits::instance()->stop(unitName);
}

QVariant HilecCore::callRcUnitMethod(const QString &unitName, const QString &method, const QList<QVariant> &params)
{
    try
    {
        QVariant retVal;
        QString errMsg;
        try
        {
            RcUnits::instance()->call(unitName.toLocal8Bit(), method.toLocal8Bit(), params);
        }
        catch(const std::exception& err)
        {
            errMsg = err.what();
        }
        if(!errMsg.isEmpty()){
            throw RcError(errMsg);
        }
        return retVal;
    }
    catch(const std::exception& err)
    {
        QString str = tr("Error calling <%1::%2>: %3").arg(unitName).arg(method).arg(err.what());
        throw std::runtime_error(qPrintable(str));
    }
}

QWidget *HilecCore::createHapticWidget(const QString &unitName)
{
    return RcUnits::instance()->createHapticWidget(unitName);
}
