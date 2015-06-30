// OFFIS Automation Framework
// Copyright (C) 2013-2014 OFFIS e.V.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "FilterSortingArea.h"
#include "ui_FilterSortingArea.h"

#include <QDragEnterEvent>
#include <QVBoxLayout>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QToolTip>
#include <QDrag>
#include <QMimeData>

#include <core/FilterInfo.h>
#include <core/OlvisInterface.h>

#include "ProcessorOutputsWidget.h"
#include "FilterOutputPortWidget.h"
#include "FilterInputPortWidget.h"
#include "FilterWidget.h"
#include "ProcessorOutputPortWidget.h"
#include "ProcessorInputPortWidget.h"

template <class Tp> Tp firstParentWidget(QWidget *w)
{
    Tp result = 0;
    while (w != 0) {
        result = qobject_cast<Tp>(w);
        if (result)
            break;
        w = w->parentWidget();
    }
    return result;
}

FilterSortingArea::FilterSortingArea(int processorId,
                                     const OlvisInterface &model,
                                     bool allowInputCreation, QWidget *parent)
    : QWidget(parent), mInterface(model), mProcessorId(processorId),
      ui(new Ui::FilterSortingArea)
{
    mRecursivePaint = false;
    ui->setupUi(this);
    mAllowInputCreate = allowInputCreation;
    ui->inputs->setAllowPortCreation(allowInputCreation);
    if (!allowInputCreation)
        ui->inputs->setVisible(false);
    setAcceptDrops(true);
    mEmptyWidget = new QWidget();
    mEmptyWidget->setFixedHeight(60);

    mCurrent.ignore = 0;

    connect(ui->outputs, SIGNAL(startConnect(FilterPortWidget *)),
            SLOT(onStartConnect(FilterPortWidget *)));
    connect(ui->outputs, SIGNAL(startDisconnect(FilterPortWidget *)),
            SLOT(onStartDisconnect(FilterPortWidget *)));
    connect(ui->inputs, SIGNAL(startConnect(FilterPortWidget *)),
            SLOT(onStartConnect(FilterPortWidget *)));
    connect(ui->inputs, SIGNAL(startDisconnect(FilterPortWidget *)),
            SLOT(onStartDisconnect(FilterPortWidget *)));

    connect(this, SIGNAL(delayedUpdateReq()), SLOT(delayedUpdate()),
            Qt::QueuedConnection);

    connect(this, SIGNAL(addFilterRequested(int, QString, int)), &mInterface,
            SLOT(addFilter(int, QString, int)), Qt::QueuedConnection);
    connect(this, SIGNAL(moveFilterRequested(int, int)), &mInterface,
            SLOT(moveFilter(int, int)), Qt::QueuedConnection);
    connect(this, SIGNAL(connectRequested(int, QString, int, QString)),
            &mInterface, SLOT(connectFilter(int, QString, int, QString)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(connectInputRequested(QString, int, QString)),
            &mInterface, SLOT(connectProcessorInput(QString, int, QString)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(disconnectRequested(int, QString)), &mInterface,
            SLOT(disconnectFilter(int, QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(addProcessorOutputRequested(int, QString)),
            &mInterface, SLOT(createProcessorOutput(int, QString)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(deleteProcessorOutputRequested(int, QString)),
            &mInterface, SLOT(deleteProcessorOutput(int, QString)),
            Qt::QueuedConnection);
}

FilterSortingArea::~FilterSortingArea() { delete ui; }

void FilterSortingArea::dragMoveEvent(QDragMoveEvent *event)
{
    dragAnimation(event);
}

void FilterSortingArea::dragEnterEvent(QDragEnterEvent *event)
{
    FilterWidget *filter = qobject_cast<FilterWidget *>(event->source());
    if (!filter)
        return;
    int moveId = filter->filterId();
    int beforeId = dragAnimation(event);
    if (moveId != 0 && !mInterface.canMove(moveId, beforeId))
        return;
    event->acceptProposedAction();
}

void FilterSortingArea::dropEvent(QDropEvent *event)
{
    FilterWidget *filter = qobject_cast<FilterWidget *>(event->source());
    if (!filter)
        return;
    int moveId = filter->filterId();
    int beforeId = dragAnimation(event);
    if (moveId != 0 && !mInterface.canMove(moveId, beforeId))
        return;
    event->acceptProposedAction();
    // need to clear the animation widget here, in case of new filter
    mEmptyWidget->hide();
    update();
    if (moveId > 0)
        emit moveFilterRequested(moveId, beforeId);
    else {
        emit addFilterRequested(mProcessorId, filter->filterType(), beforeId);
    }
}

void FilterSortingArea::dragLeaveEvent(QDragLeaveEvent *) {}

int FilterSortingArea::dragAnimation(QDropEvent *event)
{
    int spacerIndex = ui->layout->indexOf(mEmptyWidget);
    for (int i = 0; i < ui->layout->count(); i++) {
        FilterWidget *filter =
            qobject_cast<FilterWidget *>(ui->layout->itemAt(i)->widget());
        if (filter) {
            if (filter->pos().y() > event->pos().y()) {
                if (spacerIndex != i - 1) {
                    ui->layout->insertWidget(i, mEmptyWidget);
                    if (!mEmptyWidget->isVisible())
                        mEmptyWidget->setVisible(true);
                    update();
                }
                return filter->filterId();
            }
        }
    }
    int index = ui->layout->indexOf(ui->outputs);
    if (spacerIndex != index - 1) {
        ui->layout->insertWidget(index, mEmptyWidget);
        if (!mEmptyWidget->isVisible())
            mEmptyWidget->setVisible(true);
        update();
    }
    return 0;
}

void FilterSortingArea::addWidget(FilterWidget *widget)
{
    connect(widget, SIGNAL(startConnect(FilterPortWidget *)),
            SLOT(onStartConnect(FilterPortWidget *)));
    connect(widget, SIGNAL(startDisconnect(FilterPortWidget *)),
            SLOT(onStartDisconnect(FilterPortWidget *)));
    layout()->addWidget(widget);
    emit delayedUpdateReq();
}

void FilterSortingArea::moveWidget(const FilterInfo &info, int beforeId)
{
    // find both widgets
    FilterWidget *filter = 0;
    // first, find the filter
    for (int i = 0; i < ui->layout->count(); i++) {
        FilterWidget *current =
            qobject_cast<FilterWidget *>(ui->layout->itemAt(i)->widget());
        if (!current)
            continue;
        if (current->filterId() == info.id) {
            filter = current;
            break;
        }
    }
    if (!filter) // filter was not found
        return;
    ui->layout->removeWidget(filter);
    update();
    // now, find the position
    int pos = ui->layout->indexOf(ui->outputs);
    for (int i = 0; i < ui->layout->count(); i++) {
        FilterWidget *current =
            qobject_cast<FilterWidget *>(ui->layout->itemAt(i)->widget());
        if (!current)
            continue;
        if (current->filterId() == beforeId) {
            pos = i;
            break;
        }
    }
    ui->layout->insertWidget(pos, filter);
    emit delayedUpdateReq();
}

void FilterSortingArea::addOutput(const PortId &portId, const QString &name)
{
    FilterPortWidget *target =
        ui->outputs->addOutput(portId.filter, portId.port, name);
    // add the connection
    foreach (FilterOutputPortWidget *output,
             findChildren<FilterOutputPortWidget *>()) {
        if (output->filterId() == portId.filter &&
            output->portName() == portId.port) {
            mConnections[output].append(target);
            break;
        }
    }
    emit delayedUpdateReq();
}

void FilterSortingArea::renameOutput(const PortId &portId, const QString &name)
{
    ui->outputs->renameOutput(portId.filter, portId.port, name);
}

void FilterSortingArea::removeOutput(const PortId &portId)
{
    FilterPortWidget *target =
        ui->outputs->takeOutput(portId.filter, portId.port);
    if (!target)
        return;
    // remove the connection
    foreach (FilterOutputPortWidget *output,
             findChildren<FilterOutputPortWidget *>()) {
        if (output->filterId() == portId.filter &&
            output->portName() == portId.port) {
            mConnections[output].removeAll(target);
            if (mConnections[output].isEmpty())
                mConnections.remove(output);
            break;
        }
    }
    delete target;
    emit delayedUpdateReq();
}

void FilterSortingArea::updateInputs(int sourceProcessorId)
{
    QList<FilterGroupPort> ports =
        mInterface.processorOutputs(sourceProcessorId);
    QList<ProcessorInputPortWidget *> obsoletePorts =
        ui->inputs->updatePorts(ports);
    foreach (ProcessorInputPortWidget *obsolete, obsoletePorts) {
        mConnections.remove(obsolete);
        delete obsolete;
    }
    ui->inputs->setVisible(!ports.empty());
    emit delayedUpdateReq();
}

void FilterSortingArea::addInput(QString name, const PortId &port)
{

    ui->inputs->addInput(name, port.filter, mInterface.getPortInfo(port));
    addConnection(name, port);
}

void FilterSortingArea::removeInput(const PortId &id)
{
    QString name = ui->inputs->nameOf(id);
    removeConnection(name, id);
    ui->inputs->removeInput(id);
    emit delayedUpdateReq();
}

void FilterSortingArea::removeInputs() { updateInputs(-1); }

void FilterSortingArea::addConnection(const PortId &src, const PortId &target)
{
    FilterOutputPortWidget *srcWidget = 0;
    FilterInputPortWidget *targetWidget = 0;
    foreach (FilterOutputPortWidget *output,
             findChildren<FilterOutputPortWidget *>()) {
        if (output->portId() == src) {
            srcWidget = output;
            break;
        }
    }
    foreach (FilterInputPortWidget *input,
             findChildren<FilterInputPortWidget *>()) {
        if (input->portId() == target) {
            targetWidget = input;
            break;
        }
    }

    if (!srcWidget || !targetWidget)
        return;
    mConnections[srcWidget].append(targetWidget);
    emit delayedUpdateReq();
}

void FilterSortingArea::removeConnection(const PortId &src,
                                         const PortId &target)
{
    foreach (FilterPortWidget *srcWidget, mConnections.keys()) {
        if (srcWidget->portId() == src) {
            foreach (FilterPortWidget *targetWidget, mConnections[srcWidget]) {
                if (targetWidget->portId() == target) {
                    mConnections[srcWidget].removeAll(targetWidget);
                    if (mConnections[srcWidget].isEmpty())
                        mConnections.remove(srcWidget);
                    break;
                }
            }
            break;
        }
    }
    emit delayedUpdateReq();
}

void FilterSortingArea::addConnection(const QString &src, const PortId &target)
{
    ProcessorInputPortWidget *srcWidget = 0;
    FilterInputPortWidget *targetWidget = 0;
    foreach (ProcessorInputPortWidget *output,
             findChildren<ProcessorInputPortWidget *>()) {
        if (output->realName() == src) {
            srcWidget = output;
            break;
        }
    }
    foreach (FilterInputPortWidget *input,
             findChildren<FilterInputPortWidget *>()) {
        if (input->portId() == target) {
            targetWidget = input;
            break;
        }
    }

    if (!srcWidget || !targetWidget)
        return;
    mConnections[srcWidget].append(targetWidget);
    emit delayedUpdateReq();
}

void FilterSortingArea::removeConnection(const QString &src,
                                         const PortId &target)
{
    foreach (FilterPortWidget *srcWidget, mConnections.keys()) {
        ProcessorInputPortWidget *input =
            qobject_cast<ProcessorInputPortWidget *>(srcWidget);
        if (input && input->realName() == src) {
            foreach (FilterPortWidget *targetWidget, mConnections[srcWidget]) {
                if (targetWidget->portId() == target) {
                    mConnections[srcWidget].removeAll(targetWidget);
                    if (mConnections[srcWidget].isEmpty())
                        mConnections.remove(srcWidget);
                    break;
                }
            }
            break;
        }
    }
    emit delayedUpdateReq();
}

void FilterSortingArea::startDrag(FilterWidget *widget, const QPoint &hotspot)
{
    if (mInterface.isRunning())
        return;
    QDrag *drag = new QDrag(widget);
    QMimeData *mimeData = new QMimeData;
    drag->setPixmap(QPixmap::grabWidget(widget));
    drag->setHotSpot(hotspot);
    drag->setMimeData(mimeData);
    int index = ui->layout->indexOf(widget);
    if (index < 0)
        index = ui->layout->indexOf(ui->outputs);
    ui->layout->insertWidget(index, mEmptyWidget);
    mEmptyWidget->setVisible(true);
    widget->setVisible(false);
    update();
    drag->exec(Qt::MoveAction);
    mEmptyWidget->setVisible(false);
    mLastDragAnimationWidget = 0;
    if (widget->filterId() > 0)
        widget->setVisible(true);
    update();
}

void FilterSortingArea::onStartConnect(FilterPortWidget *output)
{
    qDebug() << "Start connect";
    mCurrent.source = output;
    ;
    int x = 0;
    if (!output->isLeft())
        x = output->width();
    mCurrent.start = output->mapTo(this, QPoint(x, output->height() / 2));
    ;
    mCurrent.possible = false;
    mCurrent.ignore = 0;
}

void FilterSortingArea::onStartDisconnect(FilterPortWidget *input)
{
    QMapIterator<FilterPortWidget *, PortWidgetList> connections(mConnections);
    while (connections.hasNext()) {
        const PortWidgetList &targets = connections.next().value();
        if (targets.contains(input)) {
            onStartConnect(connections.key());
            mCurrent.ignore = input;
            return;
        }
    }
}

void FilterSortingArea::delayedUpdate()
{
    QApplication::processEvents();
    update();
}

void FilterSortingArea::mouseMoveEvent(QMouseEvent *event)
{
    if (!mCurrent.start.isNull()) {
        mCurrent.end = event->pos();
        QObject *child = childAt(event->pos());
        FilterPortWidget *port = firstParentWidget<FilterPortWidget *>(
            qobject_cast<QWidget *>(child));
        mCurrent.valid = false;
        if (port) {
            mCurrent.valid = true;
            mCurrent.end = port->mapTo(this, QPoint(0, port->height() / 2));
            ;
            mCurrent.end.setX(mCurrent.start.x());
            QString warning;
            ProcessorInputPortWidget *input =
                qobject_cast<ProcessorInputPortWidget *>(mCurrent.source);
            if (input)
                mCurrent.possible =
                    port == mCurrent.ignore ||
                    mInterface.canConnectInput(input->realName(),
                                               port->portId(), warning);
            else
                mCurrent.possible =
                    port == mCurrent.ignore ||
                    mInterface.canConnect(mCurrent.source->portId(),
                                          port->portId(), warning);
            mCurrent.hasWarning = !warning.isEmpty();
            if (!warning.isEmpty())
                QToolTip::showText(mapToGlobal(event->pos()), warning);
            else
                QToolTip::hideText();
        } else
            QToolTip::hideText();

        ProcessorOutputsWidget *outputs =
            firstParentWidget<ProcessorOutputsWidget *>(
                qobject_cast<QWidget *>(child));

        if (outputs) {
            if (mInterface.canBeProcessorOutput(mCurrent.source->portId())) {
                ui->outputs->addTempPort(mCurrent.source->isLeft(),
                                         mCurrent.source->filterId(),
                                         mCurrent.source->portName());
                mCurrent.valid = true;
                mCurrent.end = ui->outputs->tempPort()->mapTo(
                    this, QPoint(0, ui->outputs->tempPort()->height() / 2));
                mCurrent.end.setX(mCurrent.start.x());
                mCurrent.possible = true; // child == ui->outputs->tempPort();
                mCurrent.hasWarning = false;
            } else {
                mCurrent.valid = true;
                mCurrent.possible = port != 0 && port == mCurrent.ignore;
            }

        } else {
            ui->outputs->clearTempPort();
        }
        update();
    }
}

void FilterSortingArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (!mCurrent.start.isNull()) {
        mCurrent.start = QPoint();
        mCurrent.end = QPoint();
        QObject *child = childAt(event->pos());
        FilterPortWidget *port = firstParentWidget<FilterPortWidget *>(
            qobject_cast<QWidget *>(child));
        ProcessorInputPortWidget *input =
            qobject_cast<ProcessorInputPortWidget *>(mCurrent.source);
        if (ui->outputs->tempPort() != 0) {
            ui->outputs->clearTempPort();
            emit addProcessorOutputRequested(mCurrent.source->filterId(),
                                             mCurrent.source->portName());
        } else if (port) {
            QString warning;
            if (input) {
                if (mInterface.canConnectInput(input->realName(),
                                               port->portId(), warning))
                    emit connectInputRequested(
                        input->realName(), port->filterId(), port->portName());
            } else if (mInterface.canConnect(mCurrent.source->portId(),
                                             port->portId(), warning))
                emit connectRequested(mCurrent.source->filterId(),
                                      mCurrent.source->portName(),
                                      port->filterId(), port->portName());
        }
        if (mCurrent.ignore && port != mCurrent.ignore) {
            ProcessorOutputPortWidget *output =
                qobject_cast<ProcessorOutputPortWidget *>(mCurrent.ignore);
            if (output)
                emit deleteProcessorOutputRequested(
                    mCurrent.ignore->filterId(), mCurrent.ignore->portName());
            else
                emit disconnectRequested(mCurrent.ignore->filterId(),
                                         mCurrent.ignore->portName());
        }
        mCurrent.ignore = 0;
        update();
    }
}

void FilterSortingArea::paintEvent(QPaintEvent *)
{
    mRecursivePaint = true;
    QPainter p(this);

    // draw existing connections
    p.setPen(QPen(Qt::blue, 2));
    // QPoint leftOffset = QPoint(-5,0);
    // QPoint rightOffset = QPoint(5,0);
    QPoint startPoint;
    // QPoint offset;
    QList<LayerUsageList> leftLayers, rightLayers;

    QMapIterator<FilterPortWidget *, PortWidgetList> connections(mConnections);
    while (connections.hasNext()) {
        connections.next();
        FilterPortWidget *src = connections.key();
        QWidget *srcParent = src->parentWidget();
        if (!srcParent || !srcParent->isVisible())
            continue;
        if (src->isVisible()) {
            if (src->isLeft())
                startPoint = src->mapTo(this, QPoint(0, src->height() / 2));
            else
                startPoint =
                    src->mapTo(this, QPoint(src->width(), src->height() / 2));
        } else {
            if (src->isLeft())
                startPoint = srcParent->mapTo(this, QPoint(0, 20));
            else
                startPoint =
                    srcParent->mapTo(this, QPoint(srcParent->width(), 20));
        }
        QListIterator<FilterPortWidget *> targets(connections.value());
        int maxY = -1;
        int startY = startPoint.y();
        QList<int> connectionPoints;
        while (targets.hasNext()) {
            FilterPortWidget *target = targets.next();
            QWidget *targetParent = target->parentWidget();
            if (!targetParent || !targetParent->isVisible() ||
                mCurrent.ignore == target)
                continue;
            QPoint p = target->mapTo(this, QPoint(0, target->height() / 2));
            if (!target->isVisible())
                p = targetParent->mapTo(this, QPoint(0, 10));

            if (p.y() > maxY) {
                if (maxY > 0)
                    connectionPoints.append(maxY);
                maxY = p.y();
            } else
                connectionPoints.append(p.y());
        }
        if (maxY < 0) // ignore connections to hidden widgets
            continue;
        // find a free layer for this connection
        int layerId = -1;
        QList<LayerUsageList> &layers =
            src->isLeft() ? leftLayers : rightLayers;
        for (int i = 0; i < layers.size(); i++) {
            layerId = i;
            QListIterator<LayerUsage> contents(layers[i]);
            while (contents.hasNext()) {
                const LayerUsage &layer = contents.next();
                bool above = maxY < layer.start;
                bool beneath = startY > layer.end;
                if (!beneath && !above) {
                    layerId = -1;
                    break;
                }
            }
            if (layerId >= 0)
                break;
        }
        if (layerId < 0) {
            layers.append(LayerUsageList());
            layerId = layers.size() - 1;
        }
        layers[layerId].append(LayerUsage(startY, maxY));

        // draw the outer connection
        QPoint offset = QPoint(16 + layerId * 8, 0);
        if (src->isLeft())
            offset = -offset;
        QPoint endPoint = QPoint(startPoint.x(), maxY);
        p.drawLine(startPoint, startPoint + offset);
        p.drawLine(endPoint, endPoint + offset);
        p.drawLine(startPoint + offset, endPoint + offset);
        // draw the dot connections
        QListIterator<int> inner(connectionPoints);
        while (inner.hasNext()) {
            QPoint point = QPoint(startPoint.x(), inner.next());
            p.drawLine(point, point + offset);
            p.drawEllipse(point + offset, 2, 2);
        }
    }
    QMargins margins = ui->layout->contentsMargins();
    int leftMargins = 16 + leftLayers.size() * 8;
    int rightMargins = 16 + rightLayers.size() * 8;
    if (margins.left() != leftMargins || margins.right() != rightMargins) {
        margins.setLeft(leftMargins);
        margins.setRight(rightMargins);
        ui->layout->setContentsMargins(margins);
        emit delayedUpdateReq();
    }

    if (!mCurrent.start.isNull() && !mCurrent.end.isNull()) {
        QColor color = Qt::black;
        if (mCurrent.valid) {
            if (mCurrent.possible) {
                if (mCurrent.hasWarning)
                    color = Qt::yellow;
                else
                    color = Qt::green;
            } else
                color = Qt::red;
        }
        p.setPen(QPen(color, 2));
        QPoint offset(8, 0);
        if (mCurrent.source->isLeft())
            offset = -offset;
        if (mCurrent.source->isLeft())
            mCurrent.start.setX(leftMargins);
        else
            mCurrent.start.setX(width() - rightMargins);
        QPoint edge(mCurrent.start.x(), mCurrent.end.y());

        p.drawLine(mCurrent.start, mCurrent.start + offset);
        p.drawLine(mCurrent.end, edge + offset);
        p.drawLine(mCurrent.start + offset, edge + offset);
    }
    mRecursivePaint = false;
}
