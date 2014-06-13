#ifndef REMOTERCUNITSERVER_H
#define REMOTERCUNITSERVER_H

#include "RemoteRcUnitServerBase.h"

class RcUnits;

class RemoteRcUnitServer : public RemoteRcUnitServerBase
{
    Q_OBJECT
public:
    RemoteRcUnitServer(RcUnits* rcUnits, QIODevice* device, bool initialize = false);

protected slots:
    void onListUnits();
    void onEnableTelecontrol(uint id, const QString& unitName);
    void onDisableTelecontrol(uint id, const QString& unitName);
    void onHandleTcData(uint id, const QMap<int, double>& data);
    void onSetTcButton(uint id, int buttonId, const bool& pressed);
    void onUpdateTcSensitivity(uint id, const QString& unitName, const QString& sensName, double sensitivity, const QList<bool>& inverts);
protected:
    RcUnits* mRcUnits;

};
#endif // REMOTERCUNITSERVER_H


