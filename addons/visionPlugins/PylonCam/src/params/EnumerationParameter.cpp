#include "EnumerationParameter.h"

EnumerationParameter::EnumerationParameter(const QString &name)
    : mName(name)
{
    mPort.setName(name);
    mPort.setVisibility(AdvancedPortVisibility);
}


bool EnumerationParameter::initialize(GenApi::INodeMap &nodes, bool ignoreCache)
{
    mIgnoreCache = ignoreCache;
    mCamParam = nodes.GetNode(mName.toLocal8Bit().data());
    if (!GenApi::IsReadable(mCamParam))
        return false;
    else {
        Pylon::StringList_t entries;
        mCamParam->GetSymbolics(entries);
        for(uint i=0;i<entries.size(); i++)
        {
            Pylon::String_t entryName = entries.at(i);
            int value = mCamParam->GetEntryByName(entryName)->GetValue();
            mPort.addChoice(value, entryName.c_str());
        }
        mPort.setDefault(mCamParam->GetIntValue(false, mIgnoreCache));
        mPort.setDesc(mCamParam->GetNode()->GetDescription().c_str());
        return true;
    }
}

bool EnumerationParameter::readable()
{
    return GenApi::IsReadable(mCamParam);
}

bool EnumerationParameter::writable()
{
    return GenApi::IsWritable(mCamParam);
}

bool EnumerationParameter::update()
{
    bool success = true;
    if(mPort.hasChanged())
    {
        if(writable())
            mCamParam->SetIntValue(mPort.getValue());
        else
            success = false;
    }
    if(mCamParam->GetIntValue(false, mIgnoreCache) != mPort.getValue())
        mPort.setDefault(mCamParam->GetIntValue());
    return success;
}
