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

#ifndef KERNELFILTER_H
#define KERNELFILTER_H

#include <filter/PluginInterface.h>
#include <ports/ImagePort.h>
#include <ports/IntegerPort.h>

template <class T1, class T2> class TemplateKernelFilter : public UserFilter
{
public:
    TemplateKernelFilter<T1,T2>()
    {
        setGroup("image/convert");

        mIn.setName("imageIn");
        mIn.setDesc(QObject::tr("Image input"));
        addInputPort(mIn);

        mOut.setName("imageOut");
        mOut.setDesc(QObject::tr("Image output"));
        addOutputPort(mOut);

        mKSize.setName("kernelSize");
        mKSize.setDesc(QObject::tr("Size of the used kernel for operation"));
        mKSize.setDefault(3);
        mKSize.setRange(1,255);
        mKSize.setOnlyOdd();

        addInputPort(mKSize);
    }

protected:
    T1 mIn;
    in::Integer mKSize;
    T2 mOut;
};

class KernelFilter : public TemplateKernelFilter<in::Image, out::Image>
{
};

#endif // KERNELFILTER_H
