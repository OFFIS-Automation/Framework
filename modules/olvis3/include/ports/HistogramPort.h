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

#ifndef HISTOGRAMPORT_H
#define HISTOGRAMPORT_H


#include "ImagePort.h"
#include "Histogram.h"

namespace out
{
    class DEFAULTPORTSSHARED_EXPORT Histogram : public port::Image
    {
    public:
        Histogram();
        virtual void send(const ::Histogram& image);
    protected:
        QVariant toSimpleType(const QVariant& var) const;

    };
}

namespace in
{
    class DEFAULTPORTSSHARED_EXPORT Histogram : public port::Image
    {
    public:
        Histogram();
        ::Histogram getValue();
        inline operator ::Histogram() { return getValue(); }
    };
}
#endif // HISTOGRAMPORT_H
