#ifndef ENUMERATIONPARAMETER_H
#define ENUMERATIONPARAMETER_H


#include "ParameterTemplate.h"
#include <QString>
#include <ports/IntegerPort.h>
#include <pylon/PylonIncludes.h>

typedef ParameterTemplate<in::Integer, GenApi::CEnumerationPtr> EnumerationParameterInterface;

/**
 * @brief EnumerationParameter encapsulates enumerations
 * enumeration values and names are read from the genicam interface
*/
class EnumerationParameter : public EnumerationParameterInterface
{
public:
    EnumerationParameter(const QString& name) : EnumerationParameterInterface(name) {}
    /**
     * @brief initialize cakks the default initalization. Additionally, enumeration values are retrieved from the Genicam itnerface
     * @see ParameterInterface and ParameterTemplate for more information
     */
    bool initialize(GenApi::INodeMap &nodes) {
        if(EnumerationParameterInterface::initialize(nodes))
        {
            Pylon::StringList_t entries;
            mCamParam->GetSymbolics(entries);
            for(uint i=0;i<entries.size(); i++)
            {
                Pylon::String_t entryName = entries.at(i);
                int value = mCamParam->GetEntryByName(entryName)->GetValue();
                mPort.addChoice(value, entryName.c_str());
            }
            // set default again, could have changed during choice setup
            readUpdate();
            return true;
        } else {
            return false;
        }
    }
    /**
     * @see ParameterTemplate
     */
    void writeNewValue() { mCamParam->SetIntValue(mPort.getValue()); }
    /**
     * @see ParameterTemplate
     */
    void readValue(bool ignoreCache, bool forceUpdate) {
        if(forceUpdate || (mCamParam->GetIntValue(false, ignoreCache) != mPort.getValue()))
            mPort.setDefault(mCamParam->GetIntValue(false, ignoreCache));
    }
};



#endif // ENUMERATIONPARAMETER_H
