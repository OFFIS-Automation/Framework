#include "BoolParameter.h"

BoolParameter::BoolParameter(const QString &name)
    : mName(name)
{
    mPort.setName(name);
    mPort.setVisibility(AdvancedPortVisibility);
}


bool BoolParameter::initialize(GenApi::INodeMap &nodes, bool ignoreCache)
{
    mIgnoreCache = ignoreCache;
    mCamParam = nodes.GetNode(mName.toLocal8Bit().data());
    if (!GenApi::IsReadable(mCamParam))
        return false;
    else {
        mPort.setDefault(mCamParam->GetValue(false, mIgnoreCache));
        mPort.setDesc(mCamParam->GetNode()->GetDescription().c_str());
        return true;
    }
}

bool BoolParameter::readable()
{
    return GenApi::IsReadable(mCamParam);
}

bool BoolParameter::writable()
{
    return GenApi::IsWritable(mCamParam);
}

bool BoolParameter::update()
{
    bool success = true;
    if(mPort.hasChanged())
    {
        if(writable())
            mCamParam->SetValue(mPort.getValue());
        else
            success = false;
    }
    if(mCamParam->GetValue(false, mIgnoreCache) != mPort.getValue())
        mPort.setDefault(mCamParam->GetValue());
    return success;
}
