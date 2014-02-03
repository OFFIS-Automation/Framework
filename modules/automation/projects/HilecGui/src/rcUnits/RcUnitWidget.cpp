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

#include "RcUnitWidget.h"
#include "ui_RcUnitWidget.h"
#include "../HilecSingleton.h"

#include <core/RcUnitHelp.h>
#include <QMenu>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>

RcUnitWidget::RcUnitWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::RcUnitWidget)
{
    ui->setupUi(this);
    connect(HilecSingleton::hilec(), SIGNAL(rcUnitsChanged(bool)), SLOT(updateRcUnits()));
    connect(this, SIGNAL(projectFileChanged(QString)), HilecSingleton::hilec(), SLOT(loadConfig(QString)), Qt::QueuedConnection);
}

RcUnitWidget::~RcUnitWidget()
{
    delete ui;
}

void RcUnitWidget::updateRcUnits()
{
    ui->units->clear();
    QMap<QString, QTreeWidgetItem*> roots;
    foreach(QString unit, HilecSingleton::hilec()->rcUnits())
    {
        const RcUnitHelp& help = HilecSingleton::hilec()->getUnitHelp(unit);
        if(!roots.contains(help.server))
            roots[help.server] = new QTreeWidgetItem(ui->units, QStringList(help.server));

        QTreeWidgetItem* item = new QTreeWidgetItem(roots[help.server], QStringList(unit));
        item->setIcon(0, QIcon(":hilecGui/lolec.png"));
    }
    ui->units->expandAll();
}

void RcUnitWidget::enableContent()
{
    ui->units->setEnabled(true);
}

void RcUnitWidget::disableContent()
{
    ui->units->setEnabled(false);
}

void RcUnitWidget::clear()
{
    ui->units->clear();
}

void RcUnitWidget::on_units_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu;
    QTreeWidgetItem* item = ui->units->itemAt(pos);

    if(item)
    {
        if(item->childCount() == 0)
            menu.addAction(ui->actionRemoveLolec);
    }
    menu.addAction(ui->actionAddLolec);
    menu.exec(mapToGlobal(pos));
}

void RcUnitWidget::on_actionAddLolec_triggered()
{
    emit addPluginToProject();
}

void RcUnitWidget::on_actionRemoveLolec_triggered()
{
    QList<QTreeWidgetItem*> items = ui->units->selectedItems();
    if(items.empty())
        return;
    QTreeWidgetItem* item = items.first();
    if(item->childCount() > 0)
        return;
    QString lolecToRemove = item->text(0);
    if(QMessageBox::question(this, tr("Remove RC-Unit"), tr("Remove the RC-Unit <%1>?").arg(lolecToRemove), QMessageBox::Yes | QMessageBox::No)
            != QMessageBox::Yes)
        return;
    QList<QVariantMap> values;
    QSettings settings(mProjectFile, QSettings::IniFormat);
    int size = settings.beginReadArray("hilecConfig");
    for(int i=0; i<size; i++)
    {
        settings.setArrayIndex(i);
        if(settings.value("name") == lolecToRemove)
            continue;
        QVariantMap elements;
        QStringList keys = settings.childKeys();
        foreach(QString key, keys)
        {
            elements[key] = settings.value(key);
        }
        values << elements;
    }
    settings.endArray();
    settings.remove("hilecConfig");
    settings.beginWriteArray("hilecConfig", values.size());
    for(int i=0;i<values.size(); i++)
    {
        settings.setArrayIndex(i);
        QVariantMap elements = values[i];
        foreach(QString key, elements.keys())
        {
            settings.setValue(key, elements.value(key));
        }
    }
    settings.sync();
    emit projectFileChanged(mProjectFile);
}
