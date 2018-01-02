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

#include <ports/HistogramPort.h>
#include <opencv2/imgproc.hpp>


Q_DECLARE_METATYPE(Histogram)


in::Histogram::Histogram() : port::Image(qMetaTypeId< :: Histogram>(), "Histogram")
{
    setIcon(QImage(":/defaultPorts/images/histogram.png"));
    setName("histogramIn");
}

Histogram in::Histogram::getValue()
{
    cv::Mat src = fromVariant(getRawValue());
    return ::Histogram(src);
}


out::Histogram::Histogram() : port::Image(qMetaTypeId< :: Histogram>(), "Histogram")
{
    setIcon(QImage(":/defaultPorts/images/histogram.png"));
    setName("histogramOut");
}

void out::Histogram::send(const ::Histogram &image)
{
    sendRaw(variant(image));
}

QVariant out::Histogram::toSimpleType(const QVariant &var) const
{
    const cv::Mat mat = fromVariant(var);
    std::vector<cv::Mat> list;
    cv::split(mat, list);
    QVariantList result;
    for(uint i=0; i<list.size(); i++)
    {
        cv::Mat channel = list[i];
        QVariantList channelVariantList;
        for(int j=0; j<channel.rows; j++)
            channelVariantList << channel.at<float>(j, 0);
        result << QVariant(channelVariantList);
    }
    return result;
}

