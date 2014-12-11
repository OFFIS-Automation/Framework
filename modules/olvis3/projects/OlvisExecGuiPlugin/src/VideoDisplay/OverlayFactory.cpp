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

#include "OverlayFactory.h"
#include "Overlay.h"
#include "FilePortOverlay.h"
#include "ImagePortOverlay.h"
#include "RectPortOverlay.h"
#include "PointPortOverlay.h"
#include "SimpleShapeOverlay.h"
#include "HistogramOverlay.h"
#include "GraphOverlay.h"
#include "SensorSystemOverlay.h"
#include "ScaleBarOverlay.h"

#include "src/OlvisSingleton.h"

#include <QDebug>

OverlayFactory::OverlayFactory()
{
    mStringOverlays.insert("String");
    mStringOverlays.insert("File");
    mStringOverlays.insert("Directory");
	mStringOverlays.insert("DateTime");
    mGraphOverlays.insert("Integer");
    mGraphOverlays.insert("Real");
    mGraphOverlays.insert("Boolean");
}

OverlayFactory& OverlayFactory::instance()
{
    static OverlayFactory inst;
    return inst;
}

Overlay* OverlayFactory::createOverlay(const QString &name)
{
    if (name == "ImagePortOverlay")
        return new ImagePortOverlay(name);
    else if (name == "StringOverlay")
        return new StringOverlay(name);
    else if (name == "RectPortOverlay")
        return new RectPortOverlay(name);
    else if (name == "FilePortOverlay")
        return new FilePortOverlay(name);
    else if (name == "SimpleShapeOverlay")
        return new SimpleShapeOverlay(name);
    else if (name == "PointPortOverlay")
        return new PointPortOverlay(name);
    else if (name == "HistogramOverlay")
        return new HistogramOverlay(name);
    else if (name == "GraphOverlay")
        return new GraphOverlay(name);
    else if (name == "SensorSystemOverlay")
        return new SensorSystemOverlay(name);
    else if(name == "ScaleBarOverlay")
        return new ScaleBarOverlay(name);
    qCritical() << tr("found unknown overlay type");
    return 0;
}

Overlay* OverlayFactory::createOverlay(const PortId &portId, bool output, bool main)
{
    const OlvisInterface& model = OlvisSingleton::instance();
    const PortInfo& info = model.getPortInfo(portId);
    QString portTypeName = info.typeName;
    Overlay* overlay = 0;
    if(!info.isArray && info.constraints.value("isPhysicalPixelSize", false).toBool() && !main)
        overlay = new ScaleBarOverlay("ScaleBarOverlay");
    else if(!info.isArray && mStringOverlays.contains(portTypeName) && !main)
        overlay = new StringOverlay("StringOverlay");
    else if(!info.isArray && mGraphOverlays.contains(portTypeName) && !main)
        overlay = new GraphOverlay("GraphOverlay");
    else if (output) {
        if (portTypeName == "Image" || portTypeName == "GrayImage" || portTypeName == "RGBImage" || portTypeName == "RGBAImage" || portTypeName == "DepthMap")
            overlay = new ImagePortOverlay("ImagePortOverlay");
        else if(portTypeName == "Histogram")
            overlay = new HistogramOverlay("HistogramOverlay");
        else if (!main && (portTypeName == "Polygon" || portTypeName == "Rect" || portTypeName == "Point" || portTypeName == "Pose2d" || portTypeName == "Line"))
            overlay = new SimpleShapeOverlay("SimpleShapeOverlay");
    } else if(!info.isArray && !main){
        if (portTypeName == "Rect")
            overlay = new RectPortOverlay("RectPortOverlay");
        else if (portTypeName == "Point")
            overlay = new PointPortOverlay("PointPortOverlay");
    }
    if (overlay)
        overlay->setPortId(portId, output);
    else
        qDebug() << "No overlay for: " << portTypeName << (output ? "output" : "input");
    return overlay;
}
