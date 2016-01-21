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

#ifndef HILECCORE_H
#define HILECCORE_H

#include <QObject>
#include <QWaitCondition>
#include <QMutex>

#include <core/HilecInterface.h>
#include <core/UserRequest.h>

#include "PythonInterpreter.h"
#include "PythonLinter.h"
#include "UserRequestManager.h"

class HilecCore : public HilecInterface
{
    Q_OBJECT
public:
    explicit HilecCore(const QString& configDir);
    static HilecCore& instance() { return *mInstance; }
    virtual ~HilecCore();
    virtual bool waitForStop(uint timeout = ULONG_MAX);
    QList<QPair<QString, int> > breakpoints() const;
    QList<int> breakpoints(const QString& filename) const;
    RcUnitHelp getUnitHelp(const QString& name);
    QStringList getTelecontrolableUnits();
    QMap<QString, Gamepad *> getGamepadDevices();
    QMap<QString, HapticDevice *> getHapticDevices();
    TelecontrolConfig getTelecontrolConfig(const QString& name);
    virtual QStringList rcUnits();
    virtual QWidget* createRcUnitWidget(const QString& rcUnit);

    virtual QAbstractItemModel* getDebugVars(int frameDepth = 0);
    virtual QList<TraceLine> getStackTrace();
    void printText(const QString& text) {  emit newTextStr(text); }
    void printError(const QString& text) {  emit newErrorStr(text); }

    void createProgress(int id, QString name, int maximum) { emit newProgressBar(id, name, maximum); }
    void updateProgress(int id, int progress) { emit progressBarUpdated(id, progress); }
    void removeProgress(int id) { emit progressBarRemoved(id); }

    void startVideoCapture(int fps, int widgetIndex) { emit videoCaptureStartRequested(fps, widgetIndex); }
    void endVideoCapture(const QString& filename, int widgetIndex) { emit videoCaptureEndRequested(filename, widgetIndex); }
    void saveScreenshot(const QString& filename, int widgetIndex) { emit saveScreenshotRequested(filename, widgetIndex); }
    void createInfoPanel(int id, const QString& title, const QStringList& names) { emit newInfoPanel(id, title, names); }
    void updateInfoPanel(int id, const QStringList& values) { emit infoPanelUpdated(id, values); }
    void removeInfoPanel(int id) { emit infoPanelRemoved(id); }
    void clearInfo() { emit infoCleared(); }
    void appendInfo(const QString& infoStr) { emit infoAppended(infoStr); }

    void raiseException(const ScriptException& err) { emit scriptException(err); }
    void raiseCompileError(const ScriptCompileInfo& err) { emit compileError(err); }
    void scriptPaused(const QString& file, int line) { emit breakpointHit(file, line); }
    QString baseDir() const { return mBaseDir; }

public slots:
    void setBaseDir(const QString& baseDir);
    void runFile(const QString &filename);
    void compileFile(const QString& filename);
    void userInput(int uid, int buttonId, const QList<QVariant>& data);
    void loadFromFile(const QString &filename);
    void releaseConfig();
    void addBreakpoint(QString file, int line);
    void removeBreakpoint(QString file, int line);
    void pause();
    void resume();
    void quit();

    void stepOver();
    void stepInto();
    void stepReturn();

    virtual void updateTelecontrolAssignment(const QString &unitName, const QString& telecontrolDeviceName);

    virtual void activateGamepad(const QString& unitName);
    virtual void deactivateGamepad(const QString& unitName);
    virtual void deactivateGamepadAll();
    virtual void updateGamepadParameters(const QString& unitName, const QString& methodName, double sensitivity, const QList<bool>& inverts);

    virtual void remoteGamepadDataUpdated(const QMap<int, double>& data, const QString& gamepadName);
    virtual void remoteGamepadButtonToggled(int buttonId, const bool& pressed, const QString& gamepadName);

    virtual void activateHaptic(const QString& unitName);
    virtual void deactivateHaptic(const QString& unitName);
    virtual void deactivateHapticAll();
    virtual void updateHapticParameters(const QString &unitName, const QString &methodName, double sensitivity, double forceScaling, const QList<bool>& inverts);

    virtual QWidget* createHapticWidget(const QString &unitName);

    virtual void callRcUnitAcquire(const QString& unitName);
    virtual void callRcUnitAcquireAll();
    virtual void callRcUnitRelease(const QString& unitName);
    virtual void callRcUnitReleaseAll();
    virtual void callRcUnitStop(const QString& unitName);
    virtual QVariant callRcUnitMethod(const QString& unitName, const QString& method, const QList<QVariant>& params);

protected:
    PythonInterpreter mPython;
    PythonLinter mLint;
    UserRequestManager mRequests;
    static HilecCore* mInstance;
    QString mBaseDir;

};

#endif // HILECCORE_H
