#ifndef FLOATPARAMETER_H
#define FLOATPARAMETER_H


#include "ParamInterface.h"
#include <QString>
#include <ports/RealPort.h>
#include <pylon/PylonIncludes.h>

class FloatParameter : public ParameterInterface
{
public:
    FloatParameter(const QString& name);
    bool initialize(GenApi::INodeMap& nodes, bool ignoreCache);
    bool readable();
    bool writable();
    Port& port() { return mPort; }
    bool update();
protected:
    QString mName;
    in::Real mPort;
    GenApi::CFloatPtr mCamParam;
    bool mIgnoreCache;
};


#endif // FLOATPARAMETER_H
