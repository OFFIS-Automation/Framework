// OFFIS Automation Framework
// Copyright (C) 2013 OFFIS e.V.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <QDebug>

#include "PylonCamera.h"

#include "params/IntegerParameter.h"
#include "params/BoolParameter.h"
#include "params/FloatParameter.h"
#include "params/EnumerationParameter.h"

using namespace Pylon;
using namespace GenApi;

PylonCamera::PylonCamera(CDeviceInfo info) : mInfo(info)
{
    setName(info.GetFriendlyName().c_str());
    setGroup(QString("input/pylon/") + info.GetDeviceClass().c_str());
    mOut.setName("output");
    addOutputPort(mOut);
}

void PylonCamera::initialize()
{
    try {
        mCam = new CInstantCamera(CTlFactory::GetInstance().CreateDevice(mInfo));
        mCam->Open();
        mCam->StopGrabbing();

        // create a subset of parameters. If a parameter is not found, it is simply not added and therefor not available
        createParam<EnumerationParameter>("PixelFormat", ExpertPortVisibility);
        createParam<IntegerParameter>("Width", AdvancedPortVisibility);
        createParam<IntegerParameter>("Height", AdvancedPortVisibility);
        createParam<IntegerParameter>("OffsetX", AdvancedPortVisibility);
        createParam<IntegerParameter>("OffsetY", AdvancedPortVisibility);
        createParam<BoolParameter>("ReverseX", ExpertPortVisibility);
        createParam<BoolParameter>("ReverseY", ExpertPortVisibility);
        createParam<EnumerationParameter>("AutoFunctionProfile", ExpertPortVisibility);
        createParam<EnumerationParameter>("GainAuto", AdvancedPortVisibility);
        // if a camera has no Gain parameter, try the GainRaw parameter
        if(!createParam<FloatParameter>("Gain", AdvancedPortVisibility)) {
            createParam<IntegerParameter>("GainRaw", AdvancedPortVisibility);
        }
        createParam<EnumerationParameter>("BalanceWhiteAuto", AdvancedPortVisibility);
        createParam<EnumerationParameter>("ShutterMode", ExpertPortVisibility);
        createParam<EnumerationParameter>("ExposureAuto", AdvancedPortVisibility);
        createParam<EnumerationParameter>("ExposureMode", ExpertPortVisibility);
        // if a camera has no ExposureTime parameter, try ExposureTimeAbs and ExposureTimeRaw
        if(!createParam<FloatParameter>("ExposureTime", AdvancedPortVisibility)) {
            if(!createParam<FloatParameter>("ExposureTimeAbs", AdvancedPortVisibility)) {
                createParam<IntegerParameter>("ExposureTimeRaw", AdvancedPortVisibility);
            }
        }
    } catch(const std::exception& e) {
        qWarning() << tr("Could not initialize pylon:") << " " <<  e.what();
        return;
    }
}

void PylonCamera::start()
{
    updateParams();
    mCam->StartGrabbing(GrabStrategy_LatestImageOnly);
}

void PylonCamera::execute()
{
    try {
        updateParams();
        CGrabResultPtr ptrGrabResult;
        mCam->RetrieveResult(500, ptrGrabResult);
        if (ptrGrabResult->GrabSucceeded()) {
            CPylonImage target;
            // convert to correct format for vision system
            CImageFormatConverter converter;
            converter.OutputPixelFormat = PixelType_BGR8packed;
            converter.OutputBitAlignment = OutputBitAlignment_MsbAligned;
            converter.Convert(target, ptrGrabResult);
            RgbImage image(target.GetHeight(), target.GetWidth(), (uchar *)target.GetBuffer());
            mOut.send(image.clone());
        }
    } catch (GenICam::GenericException &e) {
        throw std::runtime_error(e.GetDescription());
    }
}

void PylonCamera::stop()
{
    mCam->StopGrabbing();
    updateParams();
}

void PylonCamera::deinitialize()
{
    if (mCam) {
        mCam->Close();
        delete mCam;
    }
    mCam = 0;
}

void PylonCamera::updateParams()
{
    // first, write all updates. Then, read all updates. Writing one parameter might effect values of others
    foreach (ParameterInterface *param, mParams)
        param->update();
    foreach (ParameterInterface *param, mParams)
        param->readUpdate();

}

Port *PylonCamera::portByName(const QString &name)
{
    foreach (ParameterInterface *param, mParams)
    {
        if(name == param->name())
            return &param->port();
    }
    return 0;
}

template <class T> bool PylonCamera::createParam(const QString &paramName, PortVisibility vis)
{
    T *param = new T(paramName);
    if (param->initialize(mCam->GetNodeMap())) {
        mParams.append(param);
        param->port().setVisibility(vis);
        addInputPort(param->port());
        return true;
    } else {
        delete param;
        return false;
    }
}
