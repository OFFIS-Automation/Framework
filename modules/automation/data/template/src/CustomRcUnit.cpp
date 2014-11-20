#include "CustomRcUnit.h"
#include <QDebug>

CustomRcUnit::CustomRcUnit()
{
}

void CustomRcUnit::acquireHardware()
{
    //@TODO: implement
    qDebug() << "CustomRcUnit" << "connect called";
    bool success = true;
    emit connectStatusChanged(success);
}

void CustomRcUnit::releaseHardware()
{
    //@TODO: implement
    qDebug() << "CustomRcUnit" << "disconnect called";
    emit connectStatusChanged(false);
}

void CustomRcUnit::moveGamepad(double xAxis, double yAxis)
{
    //if this is connected to a gamepad method, the values for xAxis and yAxis are between -1.0 and 1.0
    //@TODO: implement
}
