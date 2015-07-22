#ifndef INTEGERPARAMETER_H
#define INTEGERPARAMETER_H

#include "ParamInterface.h"
#include "ParameterTemplate.h"
#include <QString>
#include <ports/IntegerPort.h>
#include <pylon/PylonIncludes.h>

typedef DefaultParameterTemplate<in::Integer, GenApi::CIntegerPtr> IntegerParameterInterface;

/**
 * @brief The IntegerParameter class encasulates Integer Ports and integer camera parameters
 */
class IntegerParameter : public IntegerParameterInterface
{
public:
    IntegerParameter(const QString& name) : IntegerParameterInterface(name) {}
    /**
     * @see ParameterTemplate. Additionally, setup min/max and stepping
     */
    void readUpdate(bool forceUpdate = false) {
        IntegerParameterInterface::readUpdate(forceUpdate);
        mPort.setDivConstraint(mCamParam->GetInc());
        mPort.setRange(mCamParam->GetMin(), mCamParam->GetMax());
    }
};
#endif // INTEGERPARAMETER_H
