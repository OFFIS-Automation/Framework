// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
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

#ifndef DEPTHMAPPORT_H
#define DEPTHMAPPORT_H


#include "ImagePort.h"
#include "DepthMap.h"

namespace port {
    class DEFAULTPORTSSHARED_EXPORT DepthMap : public port::Image
    {
    public:
        static QVariant variant(const ::DepthMap& mat);
        static const ::DepthMap fromVariant(const QVariant& var);
    protected:
        DepthMap();
    };
}
namespace out
{
    class DEFAULTPORTSSHARED_EXPORT DepthMap : public port::DepthMap
    {
    public:
        DepthMap();
        virtual void send(const ::DepthMap& image);

    };
}

namespace in
{
    class DEFAULTPORTSSHARED_EXPORT DepthMap : public port::DepthMap
    {
    public:
        DepthMap();
        const ::DepthMap getValue();
        inline operator ::DepthMap() { return getValue(); }
    };
}



#endif // DEPTHMAPPORT_H
