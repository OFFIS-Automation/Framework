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

#ifndef VIDEORECORDER_H
#define VIDEORECORDER_H

#include <QRect>
#include <QImage>
#include <QLinkedList>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QWaitCondition>

class VideoDisplayWidget;

class VideoRecorder : public QThread
{
    Q_OBJECT
public:
    VideoRecorder(VideoDisplayWidget* widget);
    virtual ~VideoRecorder();

	virtual void saveScreenshot(QRect rect, QString fileName);
    virtual void startVideo(QRect rect, int fps = -1);
    virtual void finishVideo();
    virtual void finishVideo(const QString& filename);
    virtual void nextFrame();
    virtual void init(QRect rect);
    virtual bool recording();

    virtual QImage getImage();
    virtual QImage getLastImage();

public slots:
    void dataChanged();
    void timerEvent();

protected:
    virtual void run();

    int mRate; // rate in ms (<=0: auto)
    int mFps;

    QRect mRect;
    VideoDisplayWidget* mWidget;
    QImage mLastImage;
    QLinkedList<QImage> mImageQueue;
    QWaitCondition mWait;
    QMutex mMutex;
    bool mFinished;

    int mVideoWriter;
    QString mTempFile;
    QThread mPaintThread;
    QTimer mTimer;
};

#endif // VIDEORECORDER_H
