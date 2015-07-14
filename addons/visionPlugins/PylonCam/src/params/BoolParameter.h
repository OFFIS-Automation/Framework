#ifndef BOOLPARAMETER_H
#define BOOLPARAMETER_H

#include "ParamInterface.h"
#include <QString>
#include <ports/BooleanPort.h>
#include <pylon/PylonIncludes.h>


class BoolParameter : public ParameterInterface
{
public:
    BoolParameter(const QString& name);
    bool initialize(GenApi::INodeMap& nodes, bool ignoreCache);
    bool readable();
    bool writable();
    Port& port() { return mPort; }
    bool update();
protected:
    QString mName;
    in::Boolean mPort;
    GenApi::CBooleanPtr mCamParam;
    bool mIgnoreCache;
};

#endif // BOOLPARAMETER_H
