#ifndef PARAMETERTEMPLATE_H
#define PARAMETERTEMPLATE_H

#include <QDebug>
#include "ParamInterface.h"
#include <QString>
#include <pylon/PylonIncludes.h>

/**
 * The ParameterTemplate class encapsulates all the shared methods of the different ports and parameters
 * For usage and Specialization, @see IntegerParameter, BoolParameter or EnumerationParameter
 * For more information about some of the methods, @see ParameterInterface
 */
template <class PORT, class PARAM> class ParameterTemplate : public ParameterInterface
{
public:
    /**
     * @param name the name of the GeniCam parameter
     */
    ParameterTemplate(const QString& name);

    /**
     * @brief tries to retrieve the port by name from the port map. On success, name and description are read and readUpdate is called
     * @return True if the port is readable, false if not
     */
    bool initialize(GenApi::INodeMap& nodes);
    bool readable() { return GenApi::IsReadable(mCamParam); }
    bool writable() { return GenApi::IsWritable(mCamParam); }
    Port& port() { return mPort; }
    /**
     * @brief checks if the port has changed and calls @see writeNewValue if neccessary
     * @return false if the port could not be written, true if write succeeded or port has no new value
     */
    bool update();
    /**
     * @brief checkes if the camera parameter cache is valid and calls @see readValue
     * @return
     */
    void readUpdate();

    /**
     * @brief writeNewValue should write the value from the port into the camera parameter
     */
    virtual void writeNewValue() = 0;
    /**
     * @brief readValue should check if the port value is different from the camera parameter value and update the ports default value if neccessary
     * @param ignoreCache if the cahce is not valid, calls to the camera parameter should ignore the cache
     */
    virtual void readValue(bool ignoreCache)  = 0;
protected:
    QString mName;
    PORT mPort;
    PARAM mCamParam;
};


/**
 * The DefaultParameterTemplate can be used if writeNewValue and readValue use the default Methods (getValue/setValue for port, GetValue/SetValue for PARAM)
 */
template <class PORT, class PARAM> class DefaultParameterTemplate : public ParameterTemplate<PORT, PARAM>
{
public:
    DefaultParameterTemplate(const QString& name) : ParameterTemplate(name) {}
    virtual void writeNewValue() { mCamParam->SetValue(mPort.getValue()); }
    virtual void readValue(bool ignoreCache);
};


template <class PORT, class PARAM>
ParameterTemplate<PORT, PARAM>::ParameterTemplate(const QString &name)
{
    mName = name;
    mPort.setName(name);
}

template <class PORT, class PARAM>
bool ParameterTemplate<PORT, PARAM>::initialize(GenApi::INodeMap &nodes)
{
    mCamParam = nodes.GetNode(mName.toLocal8Bit().data());
    if (!GenApi::IsReadable(mCamParam))
        return false;
    else {
        mPort.setDesc(mCamParam->GetNode()->GetDescription().c_str());
        readUpdate();
        return true;
    }
}

template <class PORT, class PARAM>
bool ParameterTemplate<PORT, PARAM>::update()
{
    bool success = true;
    if(mPort.hasChanged())
    {
        if(writable())
            writeNewValue();
        else
            success = false;
    }
    return success;
}

template <class PORT, class PARAM>
void ParameterTemplate<PORT, PARAM>::readUpdate()
{
    readValue(mCamParam->IsValueCacheValid());
}


template <class PORT, class PARAM>
void DefaultParameterTemplate<PORT, PARAM>::readValue(bool ignoreCache)
{
    if(mPort.getValue() != mCamParam->GetValue(false, ignoreCache))
        mPort.setDefault(mCamParam->GetValue(false, ignoreCache));
}

#endif // PARAMETERTEMPLATE_H
