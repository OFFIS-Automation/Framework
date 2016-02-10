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

#ifndef BACKGROUNDSUBSTRACTION_H
#define BACKGROUNDSUBSTRACTION_H

#include <opencv2/video.hpp>
#include <filter/PluginInterface.h>
#include <ports/ImagePort.h>
#include <ports/IntegerPort.h>
#include <ports/BooleanPort.h>

class BackgroundSubstraction : public UserFilter
{
public:
    BackgroundSubstraction();
    void execute();
protected:
    enum Mode
    {
        MOG2 = 0,
        KNN
    };

    in::Image mIn;
    in::Integer mMode;
    out::Image mBackgroundOut;
    out::Image mForegroundOut;
private:
    cv::Ptr<cv::BackgroundSubtractorKNN> mKNN;
    cv::Ptr<cv::BackgroundSubtractorMOG2> mMog2;
};

#endif // BACKGROUNDSUBSTRACTION_H
