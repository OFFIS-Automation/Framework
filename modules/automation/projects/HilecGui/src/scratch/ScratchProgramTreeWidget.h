#ifndef SCRATCHPROGRAMTREEWIDGET_H
#define SCRATCHPROGRAMTREEWIDGET_H

#include <QObject>
#include <QTreeWidget>

class ScratchProgramTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ScratchProgramTreeWidget(QWidget *parent = 0);

protected:
    void dropEvent(QDropEvent *event);
    //bool dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action);
};

#endif // SCRATCHPROGRAMTREEWIDGET_H
