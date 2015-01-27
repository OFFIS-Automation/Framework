#ifndef REMOTERCUNITCLIENT_H
#define REMOTERCUNITCLIENT_H


#include <QMap>
#include <QWaitCondition>
#include "RemoteRcUnitClientBase.h"

class RemoteRcUnitClient : public RemoteRcUnitClientBase
{
    Q_OBJECT
public:
    enum { DefaultTimeout = 0 };
    RemoteRcUnitClient(QIODevice* readDevice, QIODevice*  writeDevice, bool initialize = true);
    QVariant callMethod(const QByteArray& unit, const QByteArray& name, const QVariantList& params, unsigned long timeout = DefaultTimeout);
    QList<RcUnitHelp> listUnits(ulong timeout = DefaultTimeout);
    void setDefaultTimeout(uint timeout);
    uint defaultTimeout();

public slots:
    void enableTelecontrol(const QString& unitName, uint timeout = DefaultTimeout);
    void disableTelecontrol(const QString& unitName, uint timeout = DefaultTimeout);
    void handleTcData(const QMap<int, double>& data, uint timeout = DefaultTimeout);
    void setTcButton(int buttonId, const bool& pressed, uint timeout = DefaultTimeout);
    void updateTelecontrolAssignment(const QString &telecontrolDeviceName, uint timeout = DefaultTimeout);
    void updateGamepadParameters(const QString& unitName, const QString& tcName, double sensitivity, const QList<bool>& inverts, uint timeout = DefaultTimeout);

protected slots:
    void onMethodError(uint id, const QString& error) { insertCallResponse(id, error, true); }
    void onMethodResponse(uint id, const QVariant& value) { insertCallResponse(id, value, false); }
    void onUnitList(const QList<RcUnitHelp>& list);
    void onTcFinished(uint id);
protected:
    void waitTcCall(uint id, uint timeout);
    void insertCallResponse(uint id, QVariant value, bool isError);

    QMutex mMutex;
    uint mNextId;
    struct RemoteCall {
        RemoteCall() : isError(false) {}
        QWaitCondition waiter;
        bool isError;
        QVariant returnValue;
    };
    QMap<uint, RemoteCall*> mRemoteCalls;
    QMap<uint, QWaitCondition*> mTcCalls;
    QList<RcUnitHelp> mLastUnitList;
    QWaitCondition mListUnitWait;
    uint mDefaultTimeout;
};
#endif // REMOTERCUNITCLIENT_H
