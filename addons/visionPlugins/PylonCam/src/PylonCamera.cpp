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

#include <pylon/usb/BaslerUsbInstantCamera.h>

#include "PylonCamera.h"

#include "params/IntegerParameter.h"
#include "params/BoolParameter.h"
#include "params/FloatParameter.h"
#include "params/EnumerationParameter.h"

using namespace Pylon;
using namespace GenApi;

/*
bool IsColorCamera(CBaslerUsbInstantCamera &camera)
{
    GenApi::NodeList_t Entries;
    camera.PixelFormat.GetEntries(Entries);
    bool Result = false;

    for (size_t i = 0; i < Entries.size(); i++) {
        GenApi::INode *pNode = Entries[i];
        if (IsAvailable(pNode->GetAccessMode())) {
            GenApi::IEnumEntry *pEnum =
                dynamic_cast<GenApi::IEnumEntry *>(pNode);
            const GenICam::gcstring sym(pEnum->GetSymbolic());
            if (sym.find(GenICam::gcstring("Bayer")) !=
                GenICam::gcstring::_npos()) {
                Result = true;
                break;
            }
        }
    }
    return Result;
}
*/

PylonCamera::PylonCamera(CDeviceInfo info) : mInfo(info)
{
    setName(info.GetFriendlyName().c_str());
    setGroup(QString("input/pylon/") + info.GetDeviceClass().c_str());
    mOut.setName("output");
    addOutputPort(mOut);
}

void PylonCamera::initialize()
{
    mCam = new CInstantCamera(CTlFactory::GetInstance().CreateDevice(mInfo));
    mCam->Open();
//    NodeList_t nodes;
//    mCam->GetNodeMap().GetNodes(nodes);
//    qWarning() << "nodes" << nodes.size() << mCam->GetDeviceInfo().GetFriendlyName();
//    for(uint i=0; i<nodes.size(); i++)
//        qWarning() << nodes[i]->GetName(true).c_str() << nodes[i]->GetDisplayName();
    mCam->StopGrabbing();
    createParam<EnumerationParameter>("PixelFormat", ExpertPortVisibility);
    createParam<IntegerParameter>("OffsetX");
    createParam<IntegerParameter>("OffsetY");
    createParam<IntegerParameter>("Width");
    createParam<IntegerParameter>("Height");
    createParam<BoolParameter>("ReverseX", ExpertPortVisibility);
    createParam<BoolParameter>("ReverseY", ExpertPortVisibility);
    createParam<EnumerationParameter>("AutoFunctionProfile", ExpertPortVisibility);
    createParam<EnumerationParameter>("GainAuto");
    if(!createParam<FloatParameter>("Gain")) {
        createParam<IntegerParameter>("GainRaw");
    }
    createParam<EnumerationParameter>("BalanceWhiteAuto");
    createParam<EnumerationParameter>("ShutterMode", ExpertPortVisibility);
    createParam<EnumerationParameter>("ExposureAuto");
    createParam<EnumerationParameter>("ExposureMode", ExpertPortVisibility);
    if(!createParam<FloatParameter>("ExposureTime")) {
        if(!createParam<FloatParameter>("ExposureTimeAbs")) {
            createParam<IntegerParameter>("ExposureTimeRaw");
        }
    }
}

void PylonCamera::start()
{
    foreach (ParameterInterface *param, mParams)
        param->update();
    foreach (ParameterInterface *param, mParams)
        param->readUpdate();
    mCam->StartGrabbing();
}

void PylonCamera::execute()
{
    try {
        foreach (ParameterInterface *param, mParams)
            param->update();
        foreach (ParameterInterface *param, mParams)
            param->readUpdate();
        CGrabResultPtr ptrGrabResult;
        mCam->RetrieveResult(500, ptrGrabResult);
        if (ptrGrabResult->GrabSucceeded()) {
            CPylonImage target;
            CImageFormatConverter converter;
            converter.OutputPixelFormat = PixelType_BGRA8packed;
            converter.OutputBitAlignment = OutputBitAlignment_MsbAligned;
            converter.Convert(target, ptrGrabResult);
            RgbaImage image(target.GetHeight(), target.GetWidth(),
                            (uchar *)target.GetBuffer());
            mOut.send(image.clone());
        }
    } catch (GenICam::GenericException &e) {
        throw std::runtime_error(e.GetDescription());
    }
}

void PylonCamera::stop() {
    mCam->StopGrabbing();
}

void PylonCamera::deinitialize()
{
    if (mCam) {
        mCam->Close();
        delete mCam;
    }
    mCam = 0;
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
