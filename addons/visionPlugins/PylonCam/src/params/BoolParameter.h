#ifndef BOOLPARAMETER_H
#define BOOLPARAMETER_H

#include "ParamInterface.h"
#include <QString>
#include <ports/BooleanPort.h>
#include <pylon/PylonIncludes.h>

/**
 * @brief BoolParameter encasulates Boolean Ports and boolean camera parameters
 */
typedef DefaultParameterTemplate<in::Boolean, GenApi::CBooleanPtr> BoolParameter;


#endif // BOOLPARAMETER_H
