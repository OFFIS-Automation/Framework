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

#include "HistogramOverlay.h"
#include <ports/HistogramPort.h>

HistogramOverlay::HistogramOverlay(const QString &name)
    : MainOverlay(name)
{
    mRect = QRect(0,0, 640, 480);
    mColors << QColor(Qt::blue);
    mColors << QColor(Qt::green);
    mColors << QColor(Qt::red);
    mColors << QColor(Qt::gray);

}

QSize HistogramOverlay::size()
{
    return mRect.size();
}

void HistogramOverlay::paintContent(QPainter& p)
{
    std::vector<cv::Mat> images;
    {
        QMutexLocker lock(&mMutex);
        if(mLastValue.isValid())
        {
            cv::Mat mat = port::Image::fromVariant(mLastValue);
            cv::split(mat, images);
        }
    }
    if(images.size() > 0)
    {
        mHists.clear();
        for(int i=0; i<qMin((int)images.size(), mColors.size()); i++)
        {

            cv::normalize(images[i], images[i], 0, size().height(), cv::NORM_MINMAX);
            QPolygonF hist;
            QPointF point = mRect.bottomLeft();
            hist << point;
            double step = size().width()/(double)images[i].rows;
            for(int j=0; j<images[i].rows; j++)
            {
                point.setY(mRect.bottom() - images[i].at<float>(j));
                hist << point;
                point.setX(point.x() + step);
            }
            point.setY(mRect.bottom());
            hist << point;
            mHists << hist;
        }
    }
    for(int i=0; i<mHists.size(); i++)
    {
        QColor c = mColors[i];
        p.setPen(c);
        c.setAlpha(80);
        p.setBrush(c);
        p.drawPolygon(mHists[i]);
    }
    p.setPen(QColor(Qt::black));
    if (mHists.isEmpty())
        p.setBrush(QColor(0, 0, 0, 128));
    else
        p.setBrush(QColor(0, 0, 0, 0));
    p.drawRect(mRect);
}
