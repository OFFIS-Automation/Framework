#ifndef PARAMINTERFACE_H
#define PARAMINTERFACE_H

#include <pylon/PylonIncludes.h>
#include <ports/DefaultPortsGlobal.h>

/**
 * @brief The ParameterInterface class is an interface for port/genicam parameter collections that are used to control the camera options
 */
class ParameterInterface
{
public:
    /**
     * @brief initialize should initialize the parameter and return true if the parameter was found inside the camera, false else.
     * @param nodes the pointer to the Genicam node map
     * @return true if the node was found and could be initialized, else false
     */
    virtual bool initialize(GenApi::INodeMap &nodes) = 0;

    /**
     * @brief update is called periodically on start, stop and in every execute cycle. Update must update the camera parameter if the port value has changed
     * @return true if the camera parameter is writable, false if not
     */
    virtual bool update() = 0;

    /**
     * @brief readUpdate is called periodically on start, stop and in every execute cycle. Update must update port default value if the camera parameter value has changed
     */
    virtual void readUpdate() = 0;

    /**
     * @return a reference to the port specialization
     */
    virtual Port& port() = 0;
};

#endif // PARAMINTERFACE_H

