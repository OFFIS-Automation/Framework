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

#ifndef CONFIGURATIONWIDGET_H
#define CONFIGURATIONWIDGET_H

#include <QDockWidget>
#include <QMap>
#include <QListWidgetItem>

namespace Ui {
    class ConfigurationWidget;
}

class ConfigurationWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit ConfigurationWidget(QWidget *parent = 0);
    ~ConfigurationWidget();
signals:
    void runRequested(QString filename);
public slots:
    void setConfigFile(const QString& filename);
    void saveConfig();

    void disableContent();
    void enableContent();
private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidget_customContextMenuRequested(const QPoint &pos);
    void addFile();
    void runConfig(QListWidgetItem *item = 0);
    void editConfig(QListWidgetItem *item = 0);
    void deleteConfig(QListWidgetItem *item = 0);

    void on_listWidget_itemChanged(QListWidgetItem *item);
    void on_listWidget_fileDropped(const QString& file);
    void on_actionRun_triggered();
    void on_actionDelete_triggered();
    void on_actionRename_triggered();
    void on_actionAdd_triggered();

private:
    void keyPressEvent(QKeyEvent *event);
    QListWidgetItem* createElement(const QString& file);
    Ui::ConfigurationWidget *ui;
    QString mBaseDir, mCurrentConfig;
};

#endif // CONFIGURATIONWIDGET_H
