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

RcFlagDefinitions CustomRcUnit::rcFlagDefinitions() const
{
    RcFlagDefinitions list;
    // create a gui definition for x and y, unit is mm and 1 decimal place should be displayed
    list << RcFlagDefinition("x", "mm", 1);
    list << RcFlagDefinition("y", "mm", 1);
    return list;
}

QVariantList CustomRcUnit::rcFlags()
{
    QVariantList list;
    QMutexLocker lock(&mMutex);// ensure thread safety
    if(mIsConnected) //only return values if hardware is connected
    {
        // values must be in the same order as the definitions
        list << mPosition.x();
        list << mPosition.y();
    }
    return list;
}


void CustomRcUnit::acquire()
{
    QMutexLocker lock(&mMutex); // ensure thread safety
    if(!mIsConnected)
    {
        qDebug() << "CustomRcUnit" << "connect called";
        //@TODO: implement
        mIsConnected = true;
    }
    emit hwConnectionStatusChanged(mIsConnected);
}

void CustomRcUnit::release()
{
    QMutexLocker lock(&mMutex); // ensure thread safety
    if(mIsConnected)
    {
        qDebug() << "CustomRcUnit" << "disconnect called";
        //@TODO: implement
        mIsConnected = false;
    }
    emit hwConnectionStatusChanged(mIsConnected);
}

void CustomRcUnit::setX(double x)
{
    QMutexLocker lock(&mMutex);  // ensure thread safety
    if(!mIsConnected)
        throw std::runtime_error("Cannot set x: hardware is not connected. Call acquire first.");
    mPosition.setX(x);
}

void CustomRcUnit::setY(double y)
{
    QMutexLocker lock(&mMutex);  // ensure thread safety
    if(!mIsConnected)
        throw std::runtime_error("Cannot set y: hardware is not connected. Call acquire first.");
    mPosition.setY(y);
}

void CustomRcUnit::setXY(double x, double y)
{
    QMutexLocker lock(&mMutex);  // ensure thread safety
    if(!mIsConnected)
        throw std::runtime_error("Cannot set xy: hardware is not connected. Call acquire first.");
    mPosition = QPointF(x, y);
}

QPointF CustomRcUnit::position()
{
    QMutexLocker lock(&mMutex);  // ensure thread safety
    if(!mIsConnected)
        throw std::runtime_error("Cannot get position: hardware is not connected. Call acquire first.");
    return mPosition;
}

void CustomRcUnit::moveGamepad(double xAxis, double yAxis)
{
    QMutexLocker lock(&mMutex);  // ensure thread safety
    if(mIsConnected)
    {
        mPosition += QPointF(xAxis, yAxis);
    }

}
