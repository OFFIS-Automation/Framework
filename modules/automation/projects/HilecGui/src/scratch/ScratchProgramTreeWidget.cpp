#include "ScratchProgramTreeWidget.h"

#include <QDebug>
#include <QDropEvent>
#include <QMimeData>
#include <QStandardItemModel>

ScratchProgramTreeWidget::ScratchProgramTreeWidget(QWidget *parent) : QTreeWidget(parent)
{
    setHeaderHidden(false);
}

/*bool ScratchProgramTreeWidget::dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action)
{
    if(!data || action == Qt::IgnoreAction){
        return false;
    }

    if(data->hasFormat("application/x-qabstractitemmodeldatalist")){
        QByteArray itemData = data->data("application/x-qabstractitemmodeldatalist");
        QDataStream stream(&itemData, QIODevice::ReadOnly);
        qDebug() << itemData;

        // Decode qabstractitemmodeldatalist
        int row, col;
        QMap<int, QString> stringData;
        QMap<int, QVariant> streamValue;
        while(!stream.atEnd()){
            stream >> row >> col >> streamValue;
            stringData.insert(col, streamValue[0].toString());
        }

        QTreeWidgetItem *item;
        if(parent == NULL){
            item = new QTreeWidgetItem(this);
        } else {
            item = new QTreeWidgetItem(parent);
            parent->setExpanded(true);
        }
        item->setText(0, stringData[0]);

        return true;
    }
    return false;
}*/

void ScratchProgramTreeWidget::dropEvent(QDropEvent *event)
{
    if(event->proposedAction() == Qt::IgnoreAction){
        return;
    }

    QTreeWidget *sourceTreeWidget = dynamic_cast<QTreeWidget *>(event->source());
    QTreeWidgetItem *sourceTreeWidgetItem = sourceTreeWidget->currentItem();
    if(sourceTreeWidgetItem){
        // Check for parent
        QString rcUnit, command;
        if(sourceTreeWidgetItem->parent()){
            QTreeWidgetItem *parentTreeWidgetItem = sourceTreeWidgetItem->parent();
            rcUnit = parentTreeWidgetItem->text(0);
        }
        command = sourceTreeWidgetItem->text(0);

        DropIndicatorPosition indicatorPosition = dropIndicatorPosition();
        QTreeWidgetItem *item;

        if(itemAt(event->pos()) == NULL || indicatorPosition == OnViewport){
            item = new QTreeWidgetItem(this);
        } else {
            if(indicatorPosition == AboveItem || indicatorPosition == BelowItem){
                QTreeWidgetItem *parent = itemAt(event->pos())->parent();
                QModelIndex index = indexAt(event->pos());

                if(parent == NULL){
                    item = new QTreeWidgetItem();
                    insertTopLevelItem(indicatorPosition == AboveItem ? qMin(0, index.row()-1) : index.row()+1, item);
                } else {
                    item = new QTreeWidgetItem();
                    parent->insertChild(indicatorPosition == AboveItem ? qMin(0, index.row()-1) : index.row()+1, item);
                }
            } else if(indicatorPosition == OnItem){
                item = new QTreeWidgetItem(itemAt(event->pos()));
                itemAt(event->pos())->setExpanded(true);
            }
        }
        item->setText(0, command);
        if(!rcUnit.isEmpty()){
            item->setText(1, rcUnit);
        }
    }
}
