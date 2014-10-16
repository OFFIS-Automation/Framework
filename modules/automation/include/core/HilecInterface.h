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

#include <QString>
#include <QObject>
#include <QMap>
#include <QAbstractItemModel>
#include "ScriptException.h"

struct UserRequest;
struct ScriptCompileInfo;
struct RcUnitHelp;
struct TelecontrolConfig;
struct TelecontrolConfig;

class HilecInterface : public QObject
{
    Q_OBJECT
public:
    virtual RcUnitHelp getUnitHelp(const QString& name) = 0;
    virtual QStringList getTelecontrolableUnits() = 0;
    virtual TelecontrolConfig getTelecontrolConfig(const QString& name) = 0;
    virtual QStringList rcUnits() = 0;
    virtual QWidget* createLolecWidget(const QString& lolec) = 0;
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

    virtual void activateTelecontrol(const QString& unit) = 0;
    virtual void deactivateTelecontrol() = 0;
    virtual void updateTelecontrol(const QString& unit, const QString& methodName, double sensitivity, const QList<bool>& inverts) = 0;

    virtual void activateHaptic(const QString& unit) = 0;
    virtual void deactivateHaptic() = 0;
    virtual void updateHaptic(const QString &unit, const QString &methodName, double sensitivity, double forceScaling, const QList<bool>& inverts) = 0;
    virtual QWidget* createHapticWidget() = 0;

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
    void telecontrolUpdated(bool gamepadActive, const QString& controlledUnit);
    void telecontrolChangeSensitivityRequested(const QString& unit, bool increase);
    void hapticUpdated(bool hapticActive, const QString& controlledUnit);

    void breakpointHit(QString file, int line);
    void scriptExecutionStarted();
    void scriptExecutionFinished();

    void videoCaptureStartRequested(int frameRate, int widgetIndex);
    void videoCaptureEndRequested(const QString& filename, int widgetIndex);
    void saveScreenshotRequested(const QString& filename, int widgetIndex);
};


#endif // HILECINTERFACE_H
