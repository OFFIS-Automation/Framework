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

#include "VideoWriter.h"


REGISTER_FILTER(VideoWriter);

VideoWriter::VideoWriter()
{
    setName("VideoOutput");
    setDesc(QObject::tr("Writes incoming data to an *.avi video file"));
    setGroup("output");

    mIn.setName("imageIn");
    mIn.setDesc(QObject::tr("Image input"));
    addInputPort(mIn);

    mFileName.setName("file");
    addInputPort(mFileName);

    mFpsIn.setName("fps");
    addInputPort(mFpsIn);

    mMode.setName("compression");
    mMode.addChoice(CV_FOURCC('X','V','I','D'), "XVid");
    mMode.addChoice(CV_FOURCC('D','I','V','X'), "DivX");
    mMode.addChoice(CV_FOURCC('P','I','M','1'), "MPEG-1");
    mMode.addChoice(0, "uncompressed avi");
    mMode.setDefault(CV_FOURCC('X','V','I','D'));
    addInputPort(mMode);
    mWriter = 0;
}

void VideoWriter::start()
{
    mWriter = new cv::VideoWriter();
}

void VideoWriter::stop()
{
    if(mWriter)
        delete mWriter;
    mWriter = 0;
}

void VideoWriter::execute()
{
    const cv::Mat source = mIn;
    if(!mWriter)
        return;
    if(!mWriter->isOpened())
    {

        std::string filename = mFileName.getValue().absoluteFilePath().toStdString();
        mWriter->open(filename, mMode.getValue() ,mFpsIn.getValue(), source.size());
    }
    if(mWriter->isOpened())
    *mWriter << source.clone();
}
