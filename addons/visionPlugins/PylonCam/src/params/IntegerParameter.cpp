#include "IntegerParameter.h"

IntegerParameter::IntegerParameter(const QString &name)
    : mName(name)
{
    mPort.setName(name);
    mPort.setVisibility(AdvancedPortVisibility);
}


bool IntegerParameter::initialize(GenApi::INodeMap &nodes)
{
    mCamParam = nodes.GetNode(mName.toLocal8Bit().data());
    if (!GenApi::IsReadable(mCamParam))
        return false;
    else {
        readUpdate();
        return true;
    }
}

bool IntegerParameter::readable()
{
    return GenApi::IsReadable(mCamParam);
}

bool IntegerParameter::writable()
{
    return GenApi::IsWritable(mCamParam);
}

bool IntegerParameter::update()
{
    bool success = true;
    if(mPort.hasChanged())
    {
        if(writable())
            mCamParam->SetValue(mPort.getValue());
        else
            success = false;
    }
    if(mCamParam->GetValue(true, true) != mPort.getValue())
        mPort.setDefault(mCamParam->GetValue());
    return success;
}

void IntegerParameter::readUpdate()
{
    mPort.setDivConstraint(mCamParam->GetInc());
    mPort.setRange(mCamParam->GetMin(), mCamParam->GetMax());
    mPort.setDefault(mCamParam->GetValue());
    mPort.setDesc(mCamParam->GetNode()->GetDescription().c_str());
}
