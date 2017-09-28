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

#include "VideoRecorder.h"

#include <QPainter>
#include <QDebug>
#include <QSettings>
#include <QFileDialog>
#include <QCoreApplication>
#include <QMessageBox>
#include "VideoDisplayWidget.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

VideoRecorder::VideoRecorder(VideoDisplayWidget* widget)
  : mRate(0), mRect(), mWidget(widget), mVideoWriter(0), mPaintThread(), mTimer(&mPaintThread)
{
    mPaintThread.start();
    moveToThread(&mPaintThread);

    connect(&mTimer, SIGNAL(timeout()), this, SLOT(timerEvent()));
}

VideoRecorder::~VideoRecorder()
{
    mPaintThread.exit();
    mPaintThread.wait();
    finishVideo();
}

void VideoRecorder::init(QRect rect)
{
    mRect = rect;
}

void VideoRecorder::saveScreenshot(QRect rect, QString fileName)
{
    init(rect);
    QImage img = getImage();
    cv::Mat m(img.height(), img.width(), CV_8UC3, img.bits(), img.bytesPerLine());
    cv::Mat m2;
    cv::cvtColor(m, m2, CV_RGB2BGR);

    QSettings settings;
    QString lastFileName = settings.value("olvisexecgui/videodisplay/lastimagefile").toString();

    if(fileName.isNull()){
        fileName = QFileDialog::getSaveFileName(mWidget, tr("Save image"), lastFileName, tr("Images (*.png *.bmp *.jpg)"));
    }

    if (!fileName.isEmpty()) {
        // Check if user has given a fileextension
        if(!fileName.endsWith(".png") && !fileName.endsWith(".bmp") && !fileName.endsWith(".jpg")){
            // Append extension
            //@TODO find better way to check this, what about other extensins (TIF, TIFF, .jpeg)
            // comment: openvc does not neccessary support all file extensions. The export type is found by analyzing the ending
            fileName.append(".png");
        }
        // Save image
        settings.setValue("olvisexecgui/videodisplay/lastimagefile", fileName);
        if(!cv::imwrite(fileName.toStdString(), m2))
            QMessageBox::information(mWidget, tr("Error while saving image"), 0, QMessageBox::Ok);
    }

}

void VideoRecorder::startVideo(QRect rect, int fps)
{
    init(rect);
    qDebug() << "Starting video" << rect;
    mTempFile = QCoreApplication::applicationDirPath() + "/temp%1.avi";

    mTempFile = mTempFile.arg((long) mWidget);
    if(fps < 0)
        mFps = QSettings().value("videoCapture/fps", 15).toInt();
    else
        mFps = fps;
    mFps = qBound(5, mFps, 60);
    mRate = 1000.0/mFps;
    mTimer.start(mRate);

    qDebug() << "recording at " << mFps << "fps";
    mFinished = false;

    start();
}
void VideoRecorder::finishVideo()
{
    if(!mVideoWriter)
        return;
    QSettings settings;
    QString lastFileName = settings.value("olvisexecgui/videodisplay/lastvideofile").toString();
    QString fileName = QFileDialog::getSaveFileName(mWidget, tr("Save video"), lastFileName, tr("AVI-Videos (*.avi)"));
    if (!fileName.isEmpty()) {
        // Check if user has given a fileextension
        if(!fileName.endsWith(".avi")){
            // Append extension
            //@TODO find better way to check this, what about other extensins (.mpg, mpeg, dv, divx)
            fileName.append(".avi");
        }
        settings.setValue("olvisexecgui/videodisplay/lastvideofile", fileName);
    }
    finishVideo(fileName);
}
void VideoRecorder::finishVideo(const QString& fileName)
{
    if (mVideoWriter) {
        mMutex.lock();
        mFinished = true;
        mWait.wakeAll();
        mMutex.unlock();
        wait();
        mTimer.stop();
        if(!fileName.isEmpty())
        {
            // Save image
            if (QFile::exists(fileName))
                QFile::remove(fileName);
            if(!QFile::rename(mTempFile, fileName))
                QMessageBox::information(mWidget, tr("Error while saving video"), 0, QMessageBox::Ok);
        } else {
            QFile::remove(mTempFile);
        }
    }
}

void VideoRecorder::nextFrame()
{
    QImage img = getImage();
    QMutexLocker lock(&mMutex);
    mImageQueue.append(img);
    mWait.wakeAll();
}

QImage VideoRecorder::getImage()
{
    QImage img(mRect.size(), QImage::Format_RGB888);
    QPainter p(&img);
    //p.setRenderHint(QPainter::SmoothPixmapTransform, true);
    //p.setRenderHint(QPainter::Antialiasing, true);
    mWidget->paintContent(p, false);
    return img;
}

QImage VideoRecorder::getLastImage()
{
    return mLastImage;
}

bool VideoRecorder::recording()
{
    return mVideoWriter && !mFinished;
}

void VideoRecorder::dataChanged()
{
    if (mRate <= 0 && recording()) {
        nextFrame();
    }
}

void VideoRecorder::timerEvent()
{
    if (recording()) {
        nextFrame();
    }
}

void VideoRecorder::run()
{
    std::string filename = mTempFile.toStdString();
    int fourcc = CV_FOURCC('X','V','I','D');
    cv::Size frameSize(mRect.width(), mRect.height());
    cv::VideoWriter writer;
    writer.open(filename, fourcc, mFps, frameSize);
    mVideoWriter = 1;
    while (true) {
        QImage img;
        mMutex.lock();
        if (!mImageQueue.empty()) {
            img = mImageQueue.takeFirst();
        } else {
            if (mFinished) {
                mMutex.unlock();
                break;
            }
            mWait.wait(&mMutex);
        }
        mMutex.unlock();

        if (!img.isNull()) {
            cv::Mat m(img.height(), img.width(), CV_8UC3, img.bits(), img.bytesPerLine());
            cv::Mat m2;
            cv::cvtColor(m, m2, CV_RGB2BGR);
            writer << m2;
        }
    }
    qDebug() << "Ended";
    mVideoWriter = 0;
}
