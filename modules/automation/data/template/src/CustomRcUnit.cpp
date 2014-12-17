#include "CustomRcUnit.h"
#include <QDebug>

CustomRcUnit::CustomRcUnit()
{
    mIsConnected = false;
}

CustomRcUnit::~CustomRcUnit()
{
    release();
}


void CustomRcUnit::acquire()
{
    QMutexLocker lock(&mMutex);
    if(!mIsConnected)
    {
        //@TODO: implement
        qDebug() << "CustomRcUnit" << "connect called";
        bool success = true;
        emit hwConnectionStatusChanged(true);
        mIsConnected = true;
    }
}

void CustomRcUnit::release()
{
    QMutexLocker lock(&mMutex);
    //@TODO: implement
    qDebug() << "CustomRcUnit" << "disconnect called";
    emit hwConnectionStatusChanged(false);
    mIsConnected = false;
}

void CustomRcUnit::setX(double x)
{
    QMutexLocker lock(&mMutex);
    if(!mIsConnected)
        throw std::runtime_error("Cannot set x: hardware is not connected. Call acquire first.");
    mPosition.setX(x);
}

void CustomRcUnit::setY(double y)
{
    QMutexLocker lock(&mMutex);
    if(!mIsConnected)
        throw std::runtime_error("Cannot set y: hardware is not connected. Call acquire first.");
    mPosition.setY(y);
}

void CustomRcUnit::setXY(double x, double y)
{
    QMutexLocker lock(&mMutex);
    if(!mIsConnected)
        throw std::runtime_error("Cannot set xy: hardware is not connected. Call acquire first.");
    mPosition = QPointF(x, y);
}

QPointF CustomRcUnit::position()
{
    QMutexLocker lock(&mMutex);
    if(!mIsConnected)
        throw std::runtime_error("Cannot get position: hardware is not connected. Call acquire first.");
    return mPosition;
}

void CustomRcUnit::moveGamepad(double xAxis, double yAxis)
{
    QMutexLocker lock(&mMutex);
    //if this is connected to a gamepad method, the values for xAxis and yAxis are between -1.0 and 1.0
    //@TODO: implement
}
