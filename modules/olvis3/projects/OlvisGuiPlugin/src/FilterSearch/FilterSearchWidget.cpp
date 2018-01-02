// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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

#include "FilterSearchWidget.h"
#include "ui_FilterSearchWidget.h"
#include "FilterSearchFlowLayout.h"
#include "../ProcessorConnect/ChangeProcessorDialog.h"

#include <QDebug>
#include <QString>

#include "CreateMakroFilterDialog.h"

FilterSearchWidget::FilterSearchWidget(const OlvisInterface& model, FilterConnectWidget* dragTarget, QWidget *parent):
    QDockWidget(parent),
    ui(new Ui::FilterSearchWidget),
    mInterface(model)
{

    ui->setupUi(this);
    ui->filterTree->setDragTarget(dragTarget);
    connect(ui->searchBox, SIGNAL(textChanged(QString)), this, SLOT(filterSearch(QString)));
    connect(ui->checkDesc, SIGNAL(clicked()), this, SLOT(emptyFilterSearch()));
    connect(&mInterface, SIGNAL(filterTypesChanged()), SLOT(createNodes()));
    mInterface.connect(this, SIGNAL(createMakroFilter(QString)), SLOT(createMakroFilter(QString)), Qt::QueuedConnection);
    setMinimumWidth(200);
    this->createNodes();
}

FilterSearchWidget::~FilterSearchWidget()
{
    delete ui;
}

void FilterSearchWidget::createNodes()
{
    ui->filterTree->clear();
    mFilterList.clear();
    mFilterTypeList.clear();

    QList<FilterTypeInfo> list = mInterface.getAllFilterTypes();
    QMap<QString, QTreeWidgetItem*> groups;

    bool descIsChecked = ui->checkDesc->isChecked();
    QString search = ui->searchBox->text();

    // Add groups
    // for all nodes
    for (int i = 0; i < list.count(); i++)
    {
        FilterTypeInfo& info = list[i];
        if (((info.desc.contains(search, Qt::CaseInsensitive)
            && descIsChecked) || (info.name.contains(search, Qt::CaseInsensitive))
            || (search.isEmpty())))
        {
            if(info.group.empty())
                info.group << tr("Default", "Group name");
            QString baseGroup = info.group[0];
            if(!groups.contains(baseGroup))
            {
                QTreeWidgetItem *item = new QTreeWidgetItem();
                item->setText(0, baseGroup);
                groups[baseGroup] = item;
                item->setData(0,Qt::UserRole, true);
            }
            QTreeWidgetItem* parent = groups[baseGroup];

            // for every group of this node, except the base node
            for (int k = 1; k < info.group.size(); k++)
            {
                bool existing = false;
                int maxChildren = parent->childCount();
                for(int childId = 0; childId < maxChildren; childId ++)
                {
                    QTreeWidgetItem* child = parent->child(childId);
                    if((child->text(0) == info.group[k])
                        && child->data(0, Qt::UserRole).toBool())
                    {
                        parent = parent->child(childId);
                        existing = true;
                        break;
                    }

                }
                if (!existing)
                {
                    QTreeWidgetItem *group = new QTreeWidgetItem();
                    group->setText(0, info.group.at(k));
                    group->setData(0,Qt::UserRole, true);
                    parent->addChild(group);
                    parent = group;
                }
            }

            FilterSearchTreeItem *item = new FilterSearchTreeItem();
            item->setText(0, info.name);
            item->setFilterType(info);
            item->setToolTip(0, info.desc);
            item->setData(0,Qt::UserRole, false);
            parent->addChild(item);
            mFilterList.append(item);
            mFilterTypeList.append(info);
        }
    }

    ui->filterTree->addTopLevelItems(groups.values());
}


void FilterSearchWidget::filterSearch(QString search)
{
    // Filter
    bool descIsChecked = ui->checkDesc->isChecked();

    //hide all parent nodes
    for (int i = 0; i < mFilterList.count(); i++)
    {
        FilterSearchTreeItem * filter = mFilterList[i];
        QTreeWidgetItem * parent = filter;
        while(parent)
        {
            parent->setHidden(true);
            parent = dynamic_cast<QTreeWidgetItem *>(parent->parent());
        }
    }

    //set visibility of nodes
    for (int i = 0; i < mFilterList.count(); i++)
    {
        FilterSearchTreeItem * filter = mFilterList[i];
        FilterTypeInfo info = mFilterTypeList[i];

        //check if the node is searched
        if ((filter->text(0).contains(search, Qt::CaseInsensitive)) ||
            (descIsChecked &&
             filter->toolTip(0).contains(search, Qt::CaseInsensitive)))
        {
            QTreeWidgetItem * parent = filter;
            while(parent)
            {
                parent->setHidden(false);
                parent = dynamic_cast<QTreeWidgetItem *>(parent->parent());
            }
        }
        else
        {
            filter->setHidden(true);
        }
    }

    // Expand list
    ui->filterTree->expandAll();
}

void FilterSearchWidget::emptyFilterSearch()
{
    this->filterSearch(ui->searchBox->text());
}

void FilterSearchWidget::on_createMakroFilter_clicked()
{
    CreateMakroFilterDialog dialog;
    if(dialog.exec())
    {
        QString name = dialog.name();
        emit createMakroFilter(name);
    }
}

void FilterSearchWidget::on_createProcessor_clicked()
{
    ChangeProcessorDialog dialog(0, this);
    if(dialog.exec())
        dialog.updateProcessor();
}
