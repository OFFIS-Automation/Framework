#ifndef CUSTOMRCUNIT_H
#define CUSTOMRCUNIT_H

#include <QObject>

class CustomRcUnit : public QObject
{
    Q_OBJECT
public:
    CustomRcUnit();
public slots:
    void acquireHardware();
    void releaseHardware();
    void moveGamepad(double xAxis, double yAxis);

signals:
    void connectStatusChanged(bool connected);
};

#endif // CUSTOMRCUNIT_H
