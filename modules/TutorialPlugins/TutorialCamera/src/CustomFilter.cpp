// OFFIS Automation Framework
// Copyright (C) 2013 OFFIS e.V.
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

#include "CustomFilter.h"
#include <opencv2/imgproc/imgproc.hpp>


REGISTER_FILTER(CustomFilter);

CustomFilter::CustomFilter()
{
    setName("Tutorial Camera");
    setDesc("A virtual camera for the Framework tutorial.");
    setGroup("tutorial");
    mOut.setName("output");
    mOut.setDesc("the image of the robot scene");
    addOutputPort(mOut);
}

void CustomFilter::initialize()
{
    DataConsumer &c = SensorSystemInterface::consumer("tutorial/image");
    connect(&c, SIGNAL(updated(QVariant,double,int)), SLOT(onNewImage(QVariant)));
}

void CustomFilter::execute()
{
    QImage image;
    {
        QMutexLocker lock(&mMutex);
        if(mImg.isNull())
            mWait.wait(&mMutex, 500);
        image = mImg;
        mImg = QImage();
    }
    if(!image.isNull())
    {
        cv::Mat mat(image.height(), image.width(), CV_8UC4, image.bits());
        mOut.send(mat.clone());
    }
}

void CustomFilter::onNewImage(QVariant data)
{
    QMutexLocker lock(&mMutex);
    mImg = data.value<QImage>();
    mWait.wakeAll();
}
