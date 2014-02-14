#ifndef REMOTERCUNITCLIENT_H
#define REMOTERCUNITCLIENT_H


#include <QMap>
#include <QWaitCondition>
#include "RemoteRcUnitClientBase.h"

class RemoteRcUnitClient : public RemoteRcUnitClientBase
{
public:
    RemoteRcUnitClient(QIODevice* writeDevice, QIODevice* readDevice = 0, bool initialize = false);

    QVariant callMethod(const QByteArray& unit, const QByteArray& name, const QVariantList& params, unsigned long timeout = ULONG_MAX);
    QList<RcUnitHelp> listUnits(ulong timeout = LONG_MAX);
protected slots:

    void onMethodError(uint id, const QString& error) { insertCallResponse(id, error, true); }
    void onMethodResponse(uint id, const QVariant& value) { insertCallResponse(id, value, false); }
    void onUnitList(const QList<RcUnitHelp>& list);
protected:
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
    QList<RcUnitHelp> mLastUnitList;
    QWaitCondition mListUnitWait;
};
#endif // REMOTERCUNITCLIENT_H
