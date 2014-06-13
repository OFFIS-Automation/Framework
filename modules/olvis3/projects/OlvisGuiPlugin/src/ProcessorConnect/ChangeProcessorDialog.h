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

#ifndef CHANGEPROCESSORDIALOG_H
#define CHANGEPROCESSORDIALOG_H

#include <QDialog>

namespace Ui {
class ChangeProcessorDialog;
}

#include "../OlvisSingleton.h"

class ChangeProcessorDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ChangeProcessorDialog(int processorId = 0, QWidget *parent = 0);
    ~ChangeProcessorDialog();

    QString processorName();
    void updateProcessor();

signals:
    void createProcessor(const QString& name);
    void renameProcessor(int id, const QString& newName);
    void connectProcessor(int sourceId, int targetId);
    void disconnectProcessor(int sourceId, int targetId);
    void setPriority(int id, int priority);
    void setStartupMode(int id, bool pausedOnStartup);
    void setTriggerMode(int id, bool ignoreTrigger);

private:
    Ui::ChangeProcessorDialog *ui;
    ProcessorInfo mInfo;
    int mTriggerId;
};

#endif // CHANGEPROCESSORDIALOG_H
