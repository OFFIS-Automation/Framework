#include "MeasureOverlay.h"

#include <QMouseEvent>

MeasureOverlay::MeasureOverlay(const QString &name)
    : Overlay(name),
      mValid(false),
      mPainting(false)
{

}

MeasureOverlay::~MeasureOverlay()
{

}

void MeasureOverlay::paint(QPainter &p, bool showControls)
{
    p.setViewTransformEnabled(true);
    mTransform = p.deviceTransform();
    if (mValid) {
        QPen pen(QBrush(Qt::green), 2.0);
        pen.setCosmetic(true);
        p.setPen(pen);
        QLineF l1(mStart, mEnd);
        p.drawLine(l1);
        QLineF l2 = l1.normalVector();
        l2.setLength(20);
        QPointF center = l2.pointAt(0.5);
        l2.translate(l1.p1()-center);
        p.drawLine(l2);
        p.drawLine(l2.translated(l1.p2()-l1.p1()));
        double len = l1.length() * mLastValue.toDouble();
        QStringList unitNames = QStringList("m") <<  "mm" << "µm" << "nm";
        QString suffix = " "+ unitNames[0];
        for(int i=1; i<unitNames.size();i++)
        {
            if(len >= 1 || len < 0)
                break;
            len *= 1000;
            suffix = " "+unitNames[i];
        }
        QFont font("Arial");
        font.setPixelSize(20);
        p.setFont(font);
        QPoint offset(7, 0);
        if(l1.angle() < 90.0)
            p.drawText(QRectF(l1.pointAt(0.5) + offset, QSizeF(500, 25)), Qt::AlignLeft | Qt::AlignVCenter, QString::number(len) + suffix);
        else if(l1.angle() < 180.0)
            p.drawText(QRectF(l1.pointAt(0.5) - offset, QSizeF(-500, +25)).normalized(), Qt::AlignRight | Qt::AlignVCenter, QString::number(len) + suffix);
        else if(l1.angle() < 270.0)
            p.drawText(QRectF(l1.pointAt(0.5) - offset, QSizeF(-500, -25)).normalized(), Qt::AlignRight | Qt::AlignVCenter, QString::number(len) + suffix);
        else
        p.drawText(QRectF(l1.pointAt(0.5) + offset, QSizeF(500, -25)), Qt::AlignLeft | Qt::AlignVCenter, QString::number(len) + suffix);

    }
}

void MeasureOverlay::mousePressEvent(QMouseEvent *event)
{
    mStart = mTransform.inverted().map(event->pos());
    mPainting = true;
    mValid = false;
    mWidget->update();
}

void MeasureOverlay::mouseReleaseEvent(QMouseEvent *event)
{
    mPainting = false;
}

void MeasureOverlay::mouseMoveEvent(QMouseEvent *event, QList<QPoint> snapPoints)
{
    if(mPainting)
    {
        mEnd= mTransform.inverted().map(event->pos());
        mValid = true;
        mWidget->update();
    }
}

void MeasureOverlay::setPortId(const PortId &portId, bool output)
{
    Overlay::setPortId(portId, true);
    mIsOutput = false;
}

