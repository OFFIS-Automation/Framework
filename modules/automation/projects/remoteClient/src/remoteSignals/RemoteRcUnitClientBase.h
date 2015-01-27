//qtRemoteSignals auto generated file
//do not edit this file; if needed subclass it or create a decorator
//md5 hash: 7192336e62a417477ab91543458f0
#ifndef REMOTERCUNITCLIENTBASE_H
#define REMOTERCUNITCLIENTBASE_H

#include "RemoteSignals.h"
#include <core/RcUnitHelp.h>

class RemoteRcUnitClientBase : public RemoteSignals
{
	Q_OBJECT
public:
	RemoteRcUnitClientBase(QIODevice* writeDevice, QIODevice* readDevice = 0, bool initialize = false);
signals:
	void unitList(const QList<RcUnitHelp>& units);
	void methodError(uint callId, const QString& error);
	void methodResponse(uint callId, const QVariant& result);
	void tcFinished(uint id);

public slots:
	void listUnits();
    void callMethod(uint id, const QByteArray& unit, const QByteArray& method, const QVariantList& params);
	void enableTelecontrol(uint id, const QString& unitName);
	void disableTelecontrol(uint id, const QString& unitName);
	void handleTcData(uint id, const QMap<int, double>& data);
	void setTcButton(uint id, int buttonId, const bool& pressed);
    void updateTelecontrolAssignment(uint id, const QString &telecontrolDeviceName);
    void updateGamepadParameters(uint id, const QString &methodName, const QString& sensName, const double& sensitivity, const QList<bool>& inverts);

private:
	void processRemoteInputs(const QByteArray& data);
};

#endif //REMOTERCUNITCLIENTBASE_H
