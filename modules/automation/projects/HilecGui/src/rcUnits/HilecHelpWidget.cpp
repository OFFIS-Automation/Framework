// OFFIS Automation Framework
// Copyright (C) 2013 OFFIS e.V.
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

#include "HilecHelpWidget.h"
#include "ui_HilecHelpWidget.h"

#include "../HilecSingleton.h"
#include "RcHelpCreator.h"

#include <QList>
#include <QDir>
#include <QDebug>

HilecHelpWidget::HilecHelpWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::HilecHelpWidget)
{
    ui->setupUi(this);
    connect(HilecSingleton::hilec(), SIGNAL(rcUnitsChanged(bool)), SLOT(updateRcUnits()));
    QString name = "rc units";
    rcUnits = new QTreeWidgetItem(QStringList(name));
    ui->treeWidget->addTopLevelItem(rcUnits);
    mNavItems[name] = rcUnits;
    scanDocFolder();

    ui->searchValue->setEnabled(false);
    QPalette p = palette();
    p.setColor(QPalette::Highlight, QColor(Qt::blue).lighter(80));
    p.setColor(QPalette::HighlightedText, Qt::white);
    setPalette(p);

    ui->hideButton->setEnabled(false);
    ui->searchNext->setEnabled(false);
    ui->searchPrev->setEnabled(false);
}

HilecHelpWidget::~HilecHelpWidget()
{
    delete ui;
}

void HilecHelpWidget::on_treeWidget_itemClicked(QTreeWidgetItem *item, int)
{
    if(item->type() == RcUnitType)
    {
        RcHelpCreator creator(item->text(0));
        ui->textBrowser->setHtml(creator.data());
        ui->searchValue->setEnabled(true);
        return;
    }
    QVariant file = item->data(0, FileRole);
    if(file.type() == QVariant::String)
    {
        ui->textBrowser->setSource(QUrl::fromLocalFile(file.toString()));
        ui->searchValue->setEnabled(true);
        return;
    }
}

void HilecHelpWidget::updateRcUnits()
{
    qDeleteAll(rcUnits->takeChildren());
    QStringList types = HilecSingleton::hilec()->rcUnits();
    types.removeDuplicates();
    types.sort();
    foreach(QString type, types)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(type),RcUnitType);
        item->setData(0, Qt::UserRole, QString(type));
        rcUnits->addChild(item);
    }
    ui->treeWidget->expandItem(rcUnits);
}

void HilecHelpWidget::scanDocFolder()
{
    QDir folder = QCoreApplication::applicationDirPath() + "/plugins/hilec/doc";
    QStringList files = folder.entryList(QStringList("*.html"),QDir::Files, QDir::Name);
    foreach(QString file, files)
    {
        QStringList parts = file.split(".");
        parts.takeLast(); // remove .html
        QTreeWidgetItem* item = 0;
        QStringListIterator iter(parts);
        while(iter.hasNext())
        {
            QString part = iter.next();
            QTreeWidgetItem* tmpItem = mNavItems.value(part, 0);

            if(!tmpItem)
            {
                tmpItem = new QTreeWidgetItem(QStringList(part));
                if(item)
                    item->addChild(tmpItem);
                else
                    ui->treeWidget->addTopLevelItem(tmpItem);
                mNavItems.insert(part, tmpItem);
            }
            item = tmpItem;
        }
        if(item)
            item->setData(0, FileRole, folder.absoluteFilePath(file));
    }
}

bool HilecHelpWidget::search(bool back)
{
    QTextDocument::FindFlags flags = 0;
    if(back)
        flags |= QTextDocument::FindBackward;
    QTextEdit* edit = ui->textBrowser;
    if(!edit) return false;
    QTextCursor c = edit->document()->find(ui->searchValue->text(), edit->textCursor(), flags);
    if(!c.isNull())
    {
        edit->setTextCursor(c);
        return true;
    }
    return false;
}

void HilecHelpWidget::clearSelection()
{
    QTextEdit* edit = ui->textBrowser;
    if(!edit) return;
    QTextCursor textCursor = edit->textCursor();
    textCursor.clearSelection();
    edit->setTextCursor(textCursor);
}

void HilecHelpWidget::on_expand_clicked()
{
    ui->treeWidget->expandAll();
}

void HilecHelpWidget::on_collapse_clicked()
{
    ui->treeWidget->collapseAll();
}

void HilecHelpWidget::on_hideButton_clicked()
{
    ui->searchValue->setText("");
    // Clear selection
    clearSelection();
}

void HilecHelpWidget::on_searchNext_clicked()
{
    if(!search() && !(ui->textBrowser->document()->toPlainText().contains(ui->searchValue->text())))
        clearSelection();
}

void HilecHelpWidget::on_searchPrev_clicked()
{
    if(!search(true)  && !(ui->textBrowser->document()->toPlainText().contains(ui->searchValue->text())))
        clearSelection();
}

void HilecHelpWidget::on_searchValue_textChanged(const QString &arg1)
{
    ui->hideButton->setEnabled(arg1.length() > 0);
    ui->searchNext->setEnabled(arg1.length() > 0);
    ui->searchPrev->setEnabled(arg1.length() > 0);
}
