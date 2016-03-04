#include "PylonPlugin.h"
#include "PylonCamera.h"

#include <filter/PluginInterface.h>
#include <pylon/PylonIncludes.h>
#include <pylon/usb/BaslerUsbInstantCamera.h>

#include <QMutexLocker>

// Namespace for using pylon objects.
using namespace Pylon;

class PylonFactory : UserFilterFactoryI
{
    virtual void setup()
    {
        pylonInstances = 0;
        try {
            QMutexLocker locker(&pylonInstancesMutex);
            PylonInitialize();
            // Get the transport layer factory and query for
            // all instances of pylon devices
            CTlFactory::GetInstance().EnumerateDevices(devices);
            PylonTerminate();
        }
        catch (const GenericException &e) {
            throw std::runtime_error(e.GetDescription());
        }
        catch(const std::exception& e) {
            throw std::runtime_error(e.what());
        }
    }

    virtual int numFilters()
    {
        return devices.size();
    }

    virtual UserFilter* instance(int number)
    {
        return new PylonCamera(devices[number], &pylonInstances, &pylonInstancesMutex);
    }

    DeviceInfoList_t devices;
    QMutex pylonInstancesMutex;
    int pylonInstances;

};

PylonFactory __pylonFactory;

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(PylonPlugin, PylonPlugin);
#endif
