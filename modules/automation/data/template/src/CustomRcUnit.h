#ifndef CUSTOMRCUNIT_H
#define CUSTOMRCUNIT_H

#include <QMutex>
#include <QObject>
#include <QPointF>

#include <types/HwRcUnit.h>

class CustomRcUnit : public HwRcUnit
{
    Q_OBJECT
public:
    CustomRcUnit();
    virtual ~ CustomRcUnit();

    /**
     * This method is used to setup a graphical user interface for this unit
     * This method defines the variables that should be shown in the gui.
     */
    RcFlagDefinitions rcFlagDefinitions() const;

    /**
     * This function is called if variables should be shown in the gui
     * The function is polled periodically, but only if the unit is acquired
     * The number of returned variables must be equal to the number of flag definitions
     */
    QVariantList rcFlags();
public slots:

    /**
     * Every unit that wraps a hardware needs this method.
     * In this method, the connection to the hardware should be established.
     * The method may raise exceptions.
     * No exception should be raised if the hardware is already connected.
     * This function must emit the hwConnectionStatusChanged signal for every call.
     */
    void acquire();

    /**
     * Every unit that wraps a hardware needs this method.
     * In this method, the connection to the hardware should be established.
     * The method may raise exceptions.
     * No exception should be raised if the hardware is already released.
     * This function must emit the hwConnectionStatusChanged signal for every call.
     */
    void release();

    /**
     * Dummy example method
     */
    void setX(double x);

    /**
     * Dummy example method
     */
    void setY(double y);

    /**
     * Dummy example method
     */
    void setXY(double x, double y);

    /**
     * Dummy example method
     */
    QPointF position();

    /**
     * Dummy example for gamepad movements
     */
    void moveGamepad(double xAxis, double yAxis);
protected:
    QMutex mMutex;
    bool mIsConnected;
    QPointF mPosition;
};
#endif // CUSTOMRCUNIT_H
