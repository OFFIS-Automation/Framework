// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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

#include <ports/DepthMapPort.h>
#include <opencv2/imgproc.hpp>

Q_DECLARE_METATYPE(DepthMap)

port::DepthMap::DepthMap() :
    port::Image(qMetaTypeId< :: DepthMap>(), "DepthMap")
{
}

QVariant port::DepthMap::variant(const ::DepthMap &val)
{
    return QVariant::fromValue< ::DepthMap>(val);

}

const ::DepthMap port::DepthMap::fromVariant(const QVariant &var)
{
    return var.value< ::DepthMap>();
}



in::DepthMap::DepthMap()
{
    setIcon(QImage(":/defaultPorts/images/depthMap.png"));
    setName("depthMapIn");
}

const DepthMap in::DepthMap::getValue()
{
    return port::DepthMap::fromVariant(getRawValue());
}


out::DepthMap::DepthMap()
{
    setIcon(QImage(":/defaultPorts/images/depthMap.png"));
    setName("depthMapOut");
}

void out::DepthMap::send(const ::DepthMap &image)
{
    sendRaw(port::DepthMap::variant(image));
}


