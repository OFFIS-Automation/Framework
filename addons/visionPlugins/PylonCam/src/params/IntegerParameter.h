#ifndef INTEGERPARAMETER_H
#define INTEGERPARAMETER_H

#include "ParamInterface.h"
#include <QString>
#include <ports/IntegerPort.h>
#include <pylon/PylonIncludes.h>

class IntegerParameter : public ParameterInterface
{
public:
    IntegerParameter(const QString& name);
    bool initialize(GenApi::INodeMap& nodes);
    bool readable();
    bool writable();
    Port& port() { return mPort; }
    bool update();
protected:
    QString mName;
    in::Integer mPort;
    GenApi::CIntegerPtr mCamParam;
};

#endif // INTEGERPARAMETER_H
