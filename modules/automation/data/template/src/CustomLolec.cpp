#include "CustomLolec.h"
#include <QDebug>

CustomLolec::CustomLolec()
{
}

void CustomLolec::acquireHardware()
{
    //@TODO: implement
    qDebug() << "CustomLolec" << "connect called";
    bool success = true;
    emit connectStatusChanged(success);
}

void CustomLolec::releaseHardware()
{
    //@TODO: implement
    qDebug() << "CustomLolec" << "disconnect called";
    emit connectStatusChanged(false);
}

void CustomLolec::moveGamepad(double xAxis, double yAxis)
{
    //if this is connected to a gamepad method, the values for xAxis and yAxis are between -1.0 and 1.0
    //@TODO: implement
}
