#include "PylonPlugin.h"
#include <filter/PluginInterface.h>
#include <pylon/PylonIncludes.h>
#include <pylon/usb/BaslerUsbInstantCamera.h>
#include "PylonCamera.h"

using namespace Pylon;

class PylonFactory : UserFilterFactoryI
{
    virtual void setup()
    {
        CTlFactory::GetInstance().EnumerateDevices(devices);
    }
    virtual int numFilters() { return devices.size(); }
    virtual UserFilter* instance(int number) {
        return new PylonCamera(devices[number]);
    }
    DeviceInfoList_t devices;
    Pylon::PylonAutoInitTerm autoInitTerm;
};

PylonFactory __pylonFactory;



#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(PylonPlugin, PylonPlugin);
#endif
