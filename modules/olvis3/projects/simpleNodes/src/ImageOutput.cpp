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

#include "ImageOutput.h"
#include <opencv2/highgui.hpp>

REGISTER_FILTER(ImageOutput);
ImageOutput::ImageOutput()
{
    setName("ImageOutput");
    setDesc(QObject::tr("Writes the incoming image to the given file"));
    setGroup("output");

    mImage.setName("imageIn");
    mImage.setDesc(QObject::tr("Image input"));
    addInputPort(mImage);

    mFile.setName("filePath");
    mFile.setDesc(QObject::tr("File to write"));
    mFile.setFilter("*.jpg *.png *.bmp");
    addInputPort(mFile);
}

void ImageOutput::execute()
{
    QFileInfo path = mFile;
    const cv::Mat img = mImage;
    cv::imwrite(path.absoluteFilePath().toStdString(), img);
}
