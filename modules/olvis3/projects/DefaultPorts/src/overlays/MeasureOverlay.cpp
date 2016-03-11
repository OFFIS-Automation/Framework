#include "MeasureOverlay.h"

#include <QMouseEvent>

MeasureOverlay::MeasureOverlay(const QString &name)
    : Overlay(name),
      mValid(false),
      mPainting(false),
      mTransformValid(false)
{

}

MeasureOverlay::~MeasureOverlay()
{
    mIsOutput = true; // needed to cause overlay to diconnect port updates
}

void MeasureOverlay::paint(QPainter &p, bool showControls)
{
    Q_UNUSED(showControls);

    p.setViewTransformEnabled(true);
    mTransform = p.deviceTransform();
    if(!mTransformValid)
    {
        if(mPainting)
            mStart = mTransform.inverted().map(mStart);
        if(mValid)
            mEnd = mTransform.inverted().map(mStart);
        mTransformValid = true;
    }
    QPen pen(QBrush(Qt::green), 2.0);
    pen.setCosmetic(true);
    p.setPen(pen);
    foreach(QLineF line, mLines)
    {
        paintLine(p, line);
    }
    if (mValid) {
        paintLine(p, QLineF(mStart, mEnd));
    }
}

void MeasureOverlay::paintLine(QPainter& p, QLineF mainLine)
{
    p.drawLine(mainLine);
    QLineF normal1 = mainLine.normalVector();
    normal1.setLength(20);
    QPointF center = normal1.pointAt(0.5);
    normal1.translate(mainLine.p1()-center);
    p.drawLine(normal1);
    p.drawLine(normal1.translated(mainLine.p2()-mainLine.p1()));
    double len = mainLine.length() * mLastValue.toDouble();
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
    if(mainLine.angle() < 90.0)
        p.drawText(QRectF(mainLine.pointAt(0.5) + offset, QSizeF(500, 25)), Qt::AlignLeft | Qt::AlignVCenter, QString::number(len) + suffix);
    else if(mainLine.angle() < 180.0)
        p.drawText(QRectF(mainLine.pointAt(0.5) - offset, QSizeF(-500, +25)).normalized(), Qt::AlignRight | Qt::AlignVCenter, QString::number(len) + suffix);
    else if(mainLine.angle() < 270.0)
        p.drawText(QRectF(mainLine.pointAt(0.5) - offset, QSizeF(-500, -25)).normalized(), Qt::AlignRight | Qt::AlignVCenter, QString::number(len) + suffix);
    else
    p.drawText(QRectF(mainLine.pointAt(0.5) + offset, QSizeF(500, -25)), Qt::AlignLeft | Qt::AlignVCenter, QString::number(len) + suffix);

}

void MeasureOverlay::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        mStart = mTransform.inverted().map(QPointF(event->pos()));
        mPainting = true;
        mValid = false;
        mWidget->update();
    }

}

void MeasureOverlay::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(mPainting && mValid)
            mLines << QLineF(mStart, mEnd);
        mPainting = false;
        mValid = false;
    }
    else if(event->button() == Qt::RightButton)
    {
        mLines.clear();
    }
    mWidget->update();

}

void MeasureOverlay::mouseMoveEvent(QMouseEvent *event, QList<QPoint> snapPoints)
{
    if(mPainting)
    {
        mEnd= mTransform.inverted().map(QPointF(event->pos()));
        mValid = true;
        mWidget->update();
    }
}

void MeasureOverlay::setPortId(const PortId &portId, bool output)
{
    Q_UNUSED(output)
    Overlay::setPortId(portId, true);
    mIsOutput = false;
}

