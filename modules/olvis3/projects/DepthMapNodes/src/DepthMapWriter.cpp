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

#include "DepthMapWriter.h"

REGISTER_FILTER(DepthMapWriter);

DepthMapWriter::DepthMapWriter()
{
    setName("DepthMapWriter");
    setDesc(QObject::tr("Inverts the image"));
    setGroup("depth map");

    mOut.setName("imageOut");
    mOut.setDesc(QObject::tr("Image output"));
    addOutputPort(mOut);

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);
}

void DepthMapWriter::execute()
{
    const cv::Mat input = mIn;
	cv::Mat output = input.clone();
    unsigned char* data = (unsigned char*)output.datastart;
    while(data < output.dataend)
    {
        *data = 255-(*data);
        data++;
    }
	mOut.send(output);
}
