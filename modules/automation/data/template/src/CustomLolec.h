#ifndef CUSTOMLOLEC_H
#define CUSTOMLOLEC_H

#include <QObject>

class CustomLolec : public QObject
{
    Q_OBJECT
public:
    CustomLolec();
public slots:
    void acquireHardware();
    void releaseHardware();
    void moveGamepad(double xAxis, double yAxis);

signals:
    void connectStatusChanged(bool connected);
};

#endif // CUSTOMLOLEC_H
