#ifndef CUSTOMRCUNIT_H
#define CUSTOMRCUNIT_H


#include <QObject>
#include <QPointF>
#include <types/HwRcUnit.h>

class CustomRcUnit : public HwRcUnit
{
    Q_OBJECT
public:
    CustomRcUnit();
    virtual ~ CustomRcUnit();
public slots:
    void acquire();
    void release();
    void setX(double x);
    void setY(double y);
    void setXY(double x, double y);
    QPointF position();
    void moveGamepad(double xAxis, double yAxis);
protected:
    QMutex mMutex;
    bool mIsConnected;
    QPointF mPosition;
};
#endif // CUSTOMRCUNIT_H
