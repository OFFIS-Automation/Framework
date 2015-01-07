// OFFIS Automation Framework
// Copyright (C) 2013-2014 OFFIS e.V.
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

#include "DefaultPorts.h"

#include <QMetaType>
#include <QtPlugin>
#include <core/PortInfo.h>
#include "overlays/PointPortOverlay.h"

#include "BoolEdit.h"
#include "FileEdit.h"
#include "DirectoryEdit.h"
#include "StringEdit.h"
#include "ChoiceEdit.h"
#include "DoubleEdit.h"
#include "NumberEdit.h"
#include "RectEdit.h"
#include "PointEdit.h"
#include "DateTimeEdit.h"

#include "overlays/ImagePortOverlay.h"
#include "overlays/RectPortOverlay.h"
#include "overlays/SimpleShapeOverlay.h"
#include "overlays/HistogramOverlay.h"
#include "overlays/GraphOverlay.h"
#include "overlays/ScaleBarOverlay.h"
#include "overlays/SensorSystemOverlay.h"

DefaultPorts::DefaultPorts()
{
    mStringOverlays.insert("String");
    mStringOverlays.insert("File");
    mStringOverlays.insert("Directory");
    mStringOverlays.insert("DateTime");
    mGraphOverlays.insert("Integer");
    mGraphOverlays.insert("Real");
    mGraphOverlays.insert("Boolean");
}

PortEditInterface* DefaultPorts::portEditFor(const PortInfo& info)
{
    if(info.isArray)
        return 0;
    if(info.constraints.contains("choices"))
        return new ChoiceEdit();
    else if(info.typeName == "Boolean")
        return new BoolEdit();
    else if(info.typeName == "File")
        return new FileEdit();
    else if(info.typeName == "Directory")
        return new DirectoryEdit();
    else if(info.typeName == "String")
        return new StringEdit();
    else if(info.typeName == "Integer")
        return new NumberEdit();
    else if(info.typeName == "Real")
        return new DoubleEdit();
    else if(info.typeName == "Rect")
        return new RectEdit();
    else if(info.typeName == "Point")
        return new PointEdit();
    else if(info.typeName == "DateTime")
        return new DateTimeEdit();
    else
        return 0;
}

OverlayInterface *DefaultPorts::overlayFor(const QString &name, bool output, bool isMasterOverlay, OlvisInterface *visionInterface)
{
    Overlay* overlay = 0;
    if (name == "ImagePortOverlay")
        overlay = new ImagePortOverlay(name);
    else if (name == "StringOverlay" && !isMasterOverlay)
        overlay = new StringOverlay(name);
    else if (name == "RectPortOverlay" && !isMasterOverlay)
        overlay = new RectPortOverlay(name);
    else if (name == "FilePortOverlay" && !isMasterOverlay)
        overlay = new StringOverlay(name);
    else if (name == "SimpleShapeOverlay" && !isMasterOverlay)
        overlay = new SimpleShapeOverlay(name);
    else if (name == "HistogramOverlay")
        overlay = new HistogramOverlay(name);
    else if (name == "GraphOverlay" && !isMasterOverlay)
        overlay = new GraphOverlay(name);
    else if (name == "PointPortOverlay" && !isMasterOverlay)
        overlay = new PointPortOverlay(name);
#ifndef NO_SENSOR_SYSTEM
    else if (name == "SensorSystemOverlay" && !isMasterOverlay)
        overlay = new SensorSystemOverlay(name);
#endif
    else if(name == "ScaleBarOverlay" && !isMasterOverlay)
        overlay = new ScaleBarOverlay(name);
    if(overlay)
        overlay->setOlvisInterface(visionInterface);
    return overlay;
}

OverlayInterface *DefaultPorts::overlayFor(const PortInfo &info, bool isOutput, bool isMasterOverlay, OlvisInterface *visionInterface)
{
    Overlay* overlay = 0;
    QString portTypeName = info.typeName;

    if(!info.isArray && info.constraints.value("isPhysicalPixelSize", false).toBool() && !isMasterOverlay)
        overlay = new ScaleBarOverlay("ScaleBarOverlay");
    else if(!info.isArray && mStringOverlays.contains(portTypeName) && !isMasterOverlay)
        overlay = new StringOverlay("StringOverlay");
    else if(!info.isArray && mGraphOverlays.contains(portTypeName) && !isMasterOverlay)
        overlay = new GraphOverlay("GraphOverlay");
    else if (isOutput) {
        if (portTypeName == "Image" || portTypeName == "GrayImage" || portTypeName == "RGBImage" || portTypeName == "RGBAImage" || portTypeName == "DepthMap")
            overlay = new ImagePortOverlay("ImagePortOverlay");
        else if(portTypeName == "Histogram")
            overlay = new HistogramOverlay("HistogramOverlay");
        else if (!isMasterOverlay && (portTypeName == "Polygon" || portTypeName == "Rect" || portTypeName == "Point" || portTypeName == "Pose2d" || portTypeName == "Line"))
            overlay = new SimpleShapeOverlay("SimpleShapeOverlay");
    } else if(!info.isArray && !isMasterOverlay){
        if (portTypeName == "Rect")
            overlay = new RectPortOverlay("RectPortOverlay");
        else if (portTypeName == "Point")
            overlay = new PointPortOverlay("PointPortOverlay");
    }
    if (overlay)
        overlay->setOlvisInterface(visionInterface);
    return overlay;
}
