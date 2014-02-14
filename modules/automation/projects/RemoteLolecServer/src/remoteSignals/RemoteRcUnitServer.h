#ifndef REMOTERCUNITSERVER_H
#define REMOTERCUNITSERVER_H

#include "RemoteRcUnitServerBase.h"

class RcUnits;

class RemoteRcUnitServer : public RemoteRcUnitServerBase
{
public:
    RemoteRcUnitServer(RcUnits* rcUnits, QIODevice* device, bool initialize = false);

protected slots:
    void onListUnits();

protected:
    RcUnits* mRcUnits;

};
#endif // REMOTERCUNITSERVER_H


