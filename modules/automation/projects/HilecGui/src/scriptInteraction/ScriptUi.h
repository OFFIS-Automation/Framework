// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

#ifndef SCRIPTINPUTPANEL_H
#define SCRIPTINPUTPANEL_H

#include <QDockWidget>
#include <QMap>

struct UserRequest;
class ScriptProgress;
class ScriptInfoPanel;

namespace Ui {
    class ScriptUi;
}

class ScriptUi : public QDockWidget
{
    Q_OBJECT

public:
    explicit ScriptUi(QWidget *parent = 0);
    ~ScriptUi();
signals:
    void newUserInput(int uid, int buttonId, const QList<QVariant>& data);

protected slots:
    void onScriptFinished();

    void handleUserRequest(const UserRequest& r);
    void abortUserRequest(int id);

    void onUserInput(int uid, int buttonId, const QList<QVariant>& data);

    void onNewPrgoressBar(int id, const QString& name, int maximum);
    void onProgressBarUpdated(int id, int progress);
    void onProgressBarRemoved(int id);

    void onNewInfoPanel(int id, const QString& title, const QStringList& names);
    void onInfoPanelUpdated(int id, const QStringList& values);
    void onInfoPanelRemoved(int id);


private:
    Ui::ScriptUi *ui;
    QMap<int, QWidget*> mInputs;
    QMap<int, ScriptProgress*> mProgressBars;
    QMap<int, ScriptInfoPanel*> mInfoPanels;
};

#endif // SCRIPTINPUTPANEL_H
