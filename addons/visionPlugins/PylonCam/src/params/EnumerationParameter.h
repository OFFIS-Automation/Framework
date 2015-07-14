#ifndef ENUMERATIONPARAMETER_H
#define ENUMERATIONPARAMETER_H


#include "ParamInterface.h"
#include <QString>
#include <ports/IntegerPort.h>
#include <pylon/PylonIncludes.h>

class EnumerationParameter : public ParameterInterface
{
public:
    EnumerationParameter(const QString& name);
    bool initialize(GenApi::INodeMap& nodes, bool ignoreCache);
    bool readable();
    bool writable();
    Port& port() { return mPort; }
    bool update();
protected:
    QString mName;
    in::Integer mPort;
    GenApi::CEnumerationPtr mCamParam;
    bool mIgnoreCache;
};


#endif // ENUMERATIONPARAMETER_H
