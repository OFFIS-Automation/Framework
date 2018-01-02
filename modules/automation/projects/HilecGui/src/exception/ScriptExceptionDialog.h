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

#ifndef SCRIPTEXCEPTIONDIALOG_H
#define SCRIPTEXCEPTIONDIALOG_H

#include <QDialog>

namespace Ui {
    class ScriptExceptionDialog;
}

class QListWidgetItem;
struct ScriptException;

class ScriptExceptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScriptExceptionDialog(QWidget *parent = 0);
    ~ScriptExceptionDialog();
signals:
    void focusLine(const QString& file, int line);

protected slots:
    void showException(const ScriptException& e);
private slots:
    void on_trace_itemDoubleClicked(QListWidgetItem *item);
protected:
    void hideEvent(QHideEvent *);
private:
    Ui::ScriptExceptionDialog *ui;
    QPoint mPos;
};

#endif // SCRIPTEXCEPTIONDIALOG_H
