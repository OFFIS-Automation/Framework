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

#ifndef IMAGEPORTOVERLAY_H
#define IMAGEPORTOVERLAY_H

#include "MainOverlay.h"

#include <opencv2/core/core.hpp>
#include <QImage>

class ImagePortOverlay : public MainOverlay
{
    Q_OBJECT;
public:
    ImagePortOverlay(QString name);
    virtual void paintContent(QPainter& painter);
    virtual void writeCurrentConfig(QXmlStreamWriter& writer);
    virtual void readElement(QXmlStreamReader& reader);
    virtual QSize size();

protected:
    double mScale;
    QImage mImage;
    cv::Mat converted;
};

#endif // IMAGEPORTOVERLAY_H
