#include "ScratchWidget.h"
#include "ui_ScratchWidget.h"

#include "../HilecSingleton.h"
#include <core/RcUnitHelp.h>

#define ROOT_TYPE_OFFSET 1
#define CHILD_TYPE_OFFSET 2

ScratchWidget::ScratchWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ScratchWidget)
{
    ui->setupUi(this);

    // Content
    ui->treeWidget_2->addTopLevelItem( new QTreeWidgetItem(QStringList("repeat") << "count (int)" << "Foo", QTreeWidgetItem::UserType + ROOT_TYPE_OFFSET));
    ui->treeWidget_2->addTopLevelItem( new QTreeWidgetItem(QStringList("delay") << "seconds (int)" << "Bar", QTreeWidgetItem::UserType + ROOT_TYPE_OFFSET));
    ui->treeWidget_2->expandAll();

    // Styling


    // Signal / slot connections
    connect(HilecSingleton::hilec(), SIGNAL(rcUnitsChanged(bool)), SLOT(updateRcUnits(bool)));
    connect(this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), SLOT(onDockLocationChanged(Qt::DockWidgetArea)));


}

ScratchWidget::~ScratchWidget()
{
    delete ui;
}

void ScratchWidget::updateRcUnits(bool partial)
{
    Q_UNUSED(partial)

    HilecInterface* hilec = HilecSingleton::hilec();
    foreach(const QString& name, hilec->rcUnits()){
        RcUnitHelp help = hilec->getUnitHelp(name);
        if(!help.methods.empty()){
            QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);
            treeItem->setFlags(treeItem->flags() ^ Qt::ItemIsDragEnabled);
            treeItem->setText(0, name);

            foreach(const RcUnitHelp::Method& method, help.methods){
                if(!method.hiddenForScratch){
                    // Get Parameter names
                    QStringList parameterNames;
                    foreach (RcUnitHelp::Parameter parameter, method.parameters) {
                        parameterNames << tr("%1 (%2)").arg(parameter.name).arg(QString(parameter.typeName));
                    }

                    QTreeWidgetItem *childTreeItem = new QTreeWidgetItem(QTreeWidgetItem::UserType + CHILD_TYPE_OFFSET);
                    childTreeItem->setText(0, method.name);
                    childTreeItem->setText(1, parameterNames.isEmpty() ? tr("None") : parameterNames.join(", "));
                    childTreeItem->setText(2, method.shortDesc);
                    treeItem->addChild(childTreeItem);
                }
            }
        }
        /*QList<QVariant> params;
        HilecSingleton::hilec()->callRcUnitMethod(name, "acquire", params);*/
    }
    ui->treeWidget->expandAll();
}

void ScratchWidget::onDockLocationChanged(const Qt::DockWidgetArea &area)
{
}
