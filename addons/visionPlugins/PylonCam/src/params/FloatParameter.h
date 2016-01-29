#ifndef FLOATPARAMETER_H
#define FLOATPARAMETER_H

#include "ParameterTemplate.h"
#include <QString>
#include <ports/RealPort.h>
#include <pylon/PylonIncludes.h>

typedef DefaultParameterTemplate<in::Real, GenApi::CFloatPtr> FloatParameterInterface;

/**
 * @brief The FloatParameter class encasulates Real Ports and float camera parameters
 */
class FloatParameter : public FloatParameterInterface
{
public:
    FloatParameter(const QString& name) : FloatParameterInterface(name) {}
    /**
     * @see ParameterTemplate. Additionally, setup min/max
     */
    void readUpdate() {
        FloatParameterInterface::readUpdate();
        mPort.setRange(mCamParam->GetMin(), mCamParam->GetMax());
    }
};


#endif // FLOATPARAMETER_H
