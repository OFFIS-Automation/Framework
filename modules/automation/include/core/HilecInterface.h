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

#ifndef HILECINTERFACE_H
#define HILECINTERFACE_H

// Project headers
#include "ScriptException.h"

// QT headers
#include <QList>
#include <QString>
#include <QObject>
#include <QMap>
#include <QAbstractItemModel>


struct UserRequest;
struct ScriptCompileInfo;
struct RcUnitHelp;
struct TelecontrolConfig;
struct TelecontrolConfig;

class HapticDevice;
class Gamepad;

class HilecInterface : public QObject
{
    Q_OBJECT
public:
    virtual RcUnitHelp getUnitHelp(const QString& name) = 0;
    virtual QStringList getTelecontrolableUnits() = 0;
    virtual QMap<QString, Gamepad *> getGamepadDevices() = 0;
    virtual QMap<QString, HapticDevice *> getHapticDevices() = 0;
    virtual TelecontrolConfig getTelecontrolConfig(const QString& name) = 0;
    virtual QStringList rcUnits() = 0;
    virtual QWidget* createRcUnitWidget(const QString& rcUnit) = 0;
    virtual QAbstractItemModel* getDebugVars(int frameDepth = 0) = 0;
    virtual QList<TraceLine> getStackTrace() = 0;
    virtual QList<QPair<QString, int> > breakpoints() const = 0;
    virtual QList<int> breakpoints(const QString& fileName) const = 0;
    virtual bool waitForStop(uint timeout = ULONG_MAX) = 0;
public slots:

    virtual void runFile(const QString& filename) = 0;
    virtual void compileFile(const QString& filename) = 0;
    virtual void userInput(int uid, int buttonId, const QList<QVariant>& data) = 0;
    virtual void loadConfig(const QString& filename) = 0;
    virtual void releaseConfig() = 0;
    virtual void addBreakpoint(QString file, int line) = 0;
    virtual void removeBreakpoint(QString file, int line) = 0;
    virtual void resume() = 0;
    virtual void pause() = 0;
    virtual void quit() = 0;
    virtual void stepOver() = 0;
    virtual void stepInto() = 0;
    virtual void stepReturn() = 0;

    virtual void activateGamepad(const QString& unitName) = 0;
    virtual void deactivateGamepad() = 0;
    virtual void updateGamepadParameters(const QString& unitName, const QString& methodName, double sensitivity, const QList<bool>& inverts) = 0;
    virtual void updateGamepadAssignment(const QString &unitName, const QString& gamepadDeviceName) = 0;

    virtual void activateHaptic(const QString& unitName) = 0;
    virtual void deactivateHaptic() = 0;
    virtual void updateHapticParameters(const QString &unitName, const QString &methodName, double sensitivity, double forceScaling, const QList<bool>& inverts) = 0;
    virtual void updateHapticAssignment(const QString &unitName, const QString& hapticInterfaceName) = 0;

    virtual QWidget* createHapticWidget(const QString &unitName) = 0;

    virtual void callRcUnitAcquire(const QString& unitName) = 0;
    virtual void callRcUnitRelease(const QString& unitName) = 0;
signals:
    void newErrorStr(const QString& text);
    void newTextStr(const QString& text);
    void userInputRequested(const UserRequest& request);
    void userInputAborted(int uid);

    void newProgressBar(int id, QString name, int maximum);
    void progressBarUpdated(int id, int value);
    void progressBarRemoved(int id);

    void newInfoPanel(int id, const QString& title, const QStringList& names);
    void infoPanelUpdated(int id, const QStringList& values);
    void infoPanelRemoved(int id);

    void infoCleared();
    void infoAppended(const QString& str);
    void scriptException(const ScriptException& err);
    void compileError(const ScriptCompileInfo& err);

    void rcUnitsChanged(bool partialReload);

    void rcUnitFlagsUpdated(const QString& rcUnitName, QVariantList data);

    void gamepadUpdated(bool gamepadActive, const QString& controlledUnit);
    void gamepadChangeSensitivityRequested(const QString& unit, bool increase);

    void hapticUpdated(bool hapticActive, const QString& controlledUnit);

    void breakpointHit(QString file, int line);
    void scriptExecutionStarted();
    void scriptExecutionFinished();

    void videoCaptureStartRequested(int frameRate, int widgetIndex);
    void videoCaptureEndRequested(const QString& filename, int widgetIndex);
    void saveScreenshotRequested(const QString& filename, int widgetIndex);
};


#endif // HILECINTERFACE_H
