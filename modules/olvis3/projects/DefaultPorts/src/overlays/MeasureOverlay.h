#ifndef MEASUREOVERLAY_H
#define MEASUREOVERLAY_H

#include "Overlay.h"


class MeasureOverlay : public Overlay
{
public:
    MeasureOverlay(const QString& name);
    ~MeasureOverlay();
    virtual void setInitialPos(const QPoint&) {}
    virtual bool contains(const QPoint& p) { return false; }
    virtual void paint(QPainter& painter, bool showControls);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent*  event);
    virtual void mouseMoveEvent(QMouseEvent*  event, QList<QPoint> snapPoints);
    void setPortId(const PortId &portId, bool output);
protected:
    bool mValid, mPainting, mTransformValid;
    QList<QLineF> mLines;
    QPointF mStart, mEnd;
    QTransform mTransform;
    void paintLine(QPainter &p, QLineF line);
};

#endif // MEASUREOVERLAY_H
