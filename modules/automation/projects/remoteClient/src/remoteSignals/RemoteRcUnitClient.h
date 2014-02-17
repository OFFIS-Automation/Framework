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
    uint mDefaultTimeout;
};
#endif // REMOTERCUNITCLIENT_H
