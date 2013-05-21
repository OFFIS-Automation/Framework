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

#ifndef EDITAREA_H
#define EDITAREA_H

#include <QDockWidget>
#include <QMdiArea>
class QPlainTextEdit;

namespace Ui {
    class EditArea;
}

class EditArea : public QDockWidget
{
    Q_OBJECT

public:
    explicit EditArea(QWidget *parent = 0);
    ~EditArea();
    void setBaseDir(QString baseDir);
signals:
    void currentFileChanged(const QString& filename);
    void fileSaved(const QString& filename);
    void fileOpened(const QString& filename);

public slots:
    void openFile(QString fileName);
    void saveFile();
    void saveAll();
    void closeAll();
    void closeCurrent();
    void newFile(QString fileName);
    void renameFile(QString oldName, QString newName);
    void renameDir(QString oldName, QString newName);
    void setTabView();
    void setSubView();
    void focusLine(const QString& filename, int line);
    void closeFile(QString filename);
    void search(QString word, int number);
    void replace(QString word);
    void showFind();
    void cascade();
    void tile();

private slots:
    void closeTab(int i);
    void increaseFontSize();
    void decreaseFontSize();
    void changeFontSize(int changePoints);
    bool search(bool back = false);
    void replace();

    void on_searchNext_clicked();
    void on_searchPrev_clicked();
    void on_replace_clicked();

    void on_replaceNext_clicked();

    void on_replaceAll_clicked();

    void on_mdiArea_subWindowActivated(QMdiSubWindow *arg1);

private:
    QPlainTextEdit* currentEditor();
    Ui::EditArea *ui;
    QString mBaseDir;
};

#endif // EDITAREA_H
