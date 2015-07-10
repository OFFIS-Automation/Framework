#include "FloatParameter.h"

FloatParameter::FloatParameter(const QString &name)
    : mName(name)
{
    mPort.setName(name);
    mPort.setVisibility(AdvancedPortVisibility);
}


bool FloatParameter::initialize(GenApi::INodeMap &nodes)
{
    mCamParam = nodes.GetNode(mName.toLocal8Bit().data());
    if (!GenApi::IsReadable(mCamParam))
        return false;
    else {
        mPort.setRange(mCamParam->GetMin(), mCamParam->GetMax());
        mPort.setDefault(mCamParam->GetValue());
        mPort.setDesc(mCamParam->GetNode()->GetDescription().c_str());
        return true;
    }
}

bool FloatParameter::readable()
{
    return GenApi::IsReadable(mCamParam);
}

bool FloatParameter::writable()
{
    return GenApi::IsWritable(mCamParam);
}

bool FloatParameter::update()
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
