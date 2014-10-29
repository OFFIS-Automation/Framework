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

#include "HilecCore.h"
#include "RcUnits.h"
#include "telecontrol/TelecontrolFactory.h"

#include <QList>
#include <QDebug>
#include <QStringList>
#include <QFileInfo>

HilecCore* HilecCore::mInstance = 0;

HilecCore::HilecCore(const QString &configDir) : mPython(configDir)
{
    if(mInstance)
        throw std::runtime_error(tr("HilecCore already running").toStdString());
    mInstance = this;
    qRegisterMetaType< QList<bool> >("QList<bool>");
    qRegisterMetaType<ScriptCompileInfo>("ScriptCompileInfo");
    connect(&mRequests, SIGNAL(newUserRequest(UserRequest)), SIGNAL(userInputRequested(UserRequest)));
    connect(&mRequests, SIGNAL(requestAborted(int)), SIGNAL(userInputAborted(int)));
    connect(&mPython, SIGNAL(finished()), SIGNAL(scriptExecutionFinished()));
    connect(&mPython, SIGNAL(started()), SIGNAL(scriptExecutionStarted()));
    connect(RcUnits::instance(), SIGNAL(unitListUpdated(bool)), SIGNAL(rcUnitsChanged(bool)));
    connect(RcUnits::instance(), SIGNAL(gamepadUpdated(bool, QString)), SIGNAL(gamepadUpdated(bool, QString)));
    connect(RcUnits::instance(), SIGNAL(gamepadSensitivityChangeRequested(QString,bool)), SIGNAL(gamepadChangeSensitivityRequested(QString,bool)));
    connect(RcUnits::instance(), SIGNAL(hapticUpdated(bool, QString)), SIGNAL(hapticUpdated(bool, QString)));
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
    if(mPython.isRunning())
        resume();
    else
        mPython.start(filename, mBaseDir);
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

QMap<QString, HapticInterface *> HilecCore::getHapticInterfaces()
{
    return TelecontrolFactory::getHapticInterfaces();
}

TelecontrolConfig HilecCore::getTelecontrolConfig(const QString &name)
{
    return RcUnits::instance()->getTelecontrolConfig(name);
}

QStringList HilecCore::rcUnits()
{
    return RcUnits::instance()->unitNames();
}

void HilecCore::loadConfig(const QString &filename)
{
    mBaseDir = QFileInfo(filename).absolutePath();
    mLint.setBasePath(mBaseDir);
    RcUnits::instance()->loadConfig(filename);
}

void HilecCore::releaseConfig()
{
    RcUnits::instance()->releaseConfig();
}

QWidget* HilecCore::createLolecWidget(const QString &lolec)
{
    return RcUnits::instance()->lolecGui(lolec);
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

void HilecCore::activateGamepad(const QString &unitName)
{
    RcUnits::instance()->activateGamepad(unitName);
}

void HilecCore::deactivateGamepad()
{
    RcUnits::instance()->deactivateGamepad();
}

void HilecCore::updateGamepad(const QString &unitName, const QString &methodName, double sensitivity, const QList<bool>& inverts)
{
    RcUnits::instance()->updateGamepad(unitName, methodName, sensitivity, inverts);
}

void HilecCore::activateHaptic(const QString &unitName)
{
    RcUnits::instance()->activateHaptic(unitName);
}

void HilecCore::deactivateHaptic()
{
    RcUnits::instance()->deactivateHaptic();
}

void HilecCore::updateHapticParameters(const QString &unitName, const QString &methodName, double sensitivity, double forceScaling, const QList<bool>& inverts)
{
    RcUnits::instance()->updateHapticParameters(unitName, methodName, sensitivity, forceScaling, inverts);
}

void HilecCore::updateHapticAssignment(const QString &unitName, const QString &hapticInterfaceName)
{
    RcUnits::instance()->updateHapticAssignment(unitName, hapticInterfaceName);
}

QWidget *HilecCore::createHapticWidget(const QString &unitName)
{
    return RcUnits::instance()->createHapticWidget(unitName);
}
