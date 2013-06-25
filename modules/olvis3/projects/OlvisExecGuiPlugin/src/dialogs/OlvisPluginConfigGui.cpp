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

#include "OlvisPluginConfigGui.h"
#include "ui_OlvisPluginConfigGui.h"
#include <QMenu>
#include <QSettings>
#include <QDir>

#include "../OlvisSingleton.h"
OlvisPluginConfigGui::OlvisPluginConfigGui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OlvisPluginConfigGui)
{
    ui->setupUi(this);
    ui->disabledFilters->setModel(&mDisabledFilters);
    QSettings settings;
    QStringList list = settings.value("olvisCore/disabledPlugins").toStringList();
    mDisabledFilters.setStringList(list);
    // path to the plugins
    QDir dir(QCoreApplication::applicationDirPath()+"/plugins/olvisPlugins");
    QStringList suffixFilter = QStringList("*.dll") << "*.so" << "*.dylib";
#ifdef Q_OS_LINUX
    QStringList files = dir.entryList(suffixFilter, QDir::Files | QDir::NoDotAndDotDot, QDir::Name | QDir::IgnoreCase);
#else
    QStringList files = dir.entryList(suffixFilter, QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDir::Name | QDir::IgnoreCase);
#endif

    QStringList plugins;
    foreach(const QString& file, files)
    {
        plugins << QFileInfo(file).baseName();
    }
    plugins.removeDuplicates();
    ui->comboBox->addItems(plugins);

    // Hide help button
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

OlvisPluginConfigGui::~OlvisPluginConfigGui()
{
    delete ui;
}


void OlvisPluginConfigGui::on_removeSelection_clicked()
{
    QStringList current = mDisabledFilters.stringList();
    QItemSelectionModel* selection = ui->disabledFilters->selectionModel();
    QStringList newList;
    for(int i=0; i<current.size(); i++)
    {
        if(!selection->isSelected(mDisabledFilters.index(i)))
            newList << current[i];
    }
    mDisabledFilters.setStringList(newList);
    QSettings settings;
    settings.setValue("olvisCore/disabledPlugins", newList);
}

void OlvisPluginConfigGui::on_add_clicked()
{
   QStringList current = mDisabledFilters.stringList();
   current << ui->comboBox->currentText();
   ui->comboBox->clearEditText();
   current.removeDuplicates();
   QSettings settings;
   settings.setValue("olvisCore/disabledPlugins", current);
   mDisabledFilters.setStringList(current);
}
