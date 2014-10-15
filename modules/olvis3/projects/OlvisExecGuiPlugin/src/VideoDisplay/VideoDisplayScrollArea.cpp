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

#include "VideoDisplayScrollArea.h"
#include "VideoDisplayWidget.h"
#include <QScrollBar>
#include <QResizeEvent>
#include <QDebug>

VideoDisplayScrollArea::VideoDisplayScrollArea(QWidget *parent) :
    QScrollArea(parent)
{
}

void VideoDisplayScrollArea::scrollContentsBy(int , int )
{
    VideoDisplayWidget* w = qobject_cast<VideoDisplayWidget*>(widget());
    w->setOffset(horizontalScrollBar()->value(), verticalScrollBar()->value());
    w->update();
}

void VideoDisplayScrollArea::resizeEvent(QResizeEvent*)
{
    VideoDisplayWidget* w = qobject_cast<VideoDisplayWidget*>(widget());
    w->resize(viewport()->size());
    //mVideoDisplayWidget->updateSize();
    w->update();
}
