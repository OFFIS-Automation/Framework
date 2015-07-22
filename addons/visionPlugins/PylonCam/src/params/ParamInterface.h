#ifndef PARAMINTERFACE_H
#define PARAMINTERFACE_H

#include <ports/DefaultPortsGlobal.h>

class ParameterInterface
{
public:
    virtual bool update() = 0;
    virtual void readUpdate() {}
    virtual Port& port() = 0;
};

#endif // PARAMINTERFACE_H

