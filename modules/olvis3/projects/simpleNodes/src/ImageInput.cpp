// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

#include "ImageInput.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdexcept>

REGISTER_FILTER(ImageInput);

ImageInput::ImageInput()
{
    setName("ImageInput");
    setDesc("Reads a single image from disc and outputs it");
    setGroup("input");
    mOut.setName("outImage");
    mOut.setDesc("Image output");
    addOutputPort(mOut);
    mIn.setName("filename");
    mIn.setDesc("File to read");
    mIn.setFilter("*.jpg *.png *.bmp");
    mAlwaysRead.setName("alwaysReload");
    mAlwaysRead.setDefault(false);
    mAlwaysRead.setDesc("Read the file from disc every time");
    mAlwaysRead.setVisibility(AdvancedPortVisibility);
    addInputPort(mIn);
    addInputPort(mAlwaysRead);
}

void ImageInput::start()
{
    lastInfo = QFileInfo();
}

void ImageInput::execute()
{

    QFileInfo path = mIn;
    if(path != lastInfo || mAlwaysRead.getValue())
    {
        image = cv::imread(path.canonicalFilePath().toStdString(), -1);
        lastInfo = path;
    }
    if(!image.empty()) // if matrix is valid
        mOut.send(image);
    else
        throw std::runtime_error(QString("Could not load image: %1").arg(path.absoluteFilePath()).toStdString());
}
