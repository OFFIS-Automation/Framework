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

#ifndef MAINTOOLBAR_H
#define MAINTOOLBAR_H

#include <QToolBar>
#include <QActionGroup>
#include <QMenu>

namespace Ui {
    class MainToolBar;
}
class OlvisInterface;

class MainToolBar : public QToolBar
{
    Q_OBJECT

public:
    explicit MainToolBar(const OlvisInterface& model, QWidget *parent = 0);
    ~MainToolBar();
    void initMenu(QMenu* fileMenu, QMenu* olvisMenu);
    void updateRecentFiles(QStringList files);
signals:
    void startRequested();
    void stopRequested();
    void pauseRequested();
    void stepRequested(bool singleStep);
    void restoreRequested();
    void saveRequested();
    void setNumDisplays(int numOfDisplays);
public slots:
    bool check();

private slots:
    void on_actionProblems_triggered();
    void on_actionStop_triggered();
    void on_actionRun_triggered();

    void on_actionPause_triggered();

    void onExecutionFinished();
    void onExecutionStarted();

    void on_actionStep_triggered();

    void on_actionStepSingle_triggered();
    void on_actionExportTrace_triggered();

    void on_actionLoadedPlugins_triggered();

    void onNumDisplaysChanged();

    void on_actionRestore_triggered();

    void on_actionSave_triggered();

private:
    Ui::MainToolBar *ui;
    const OlvisInterface& mInterface;
    QString mLastSaveFile;
    QString mLastLoadFile;
    QActionGroup* mRunActionGroup, *mDisplayActionGroup;
    int mActiveProcessor;
    QMenu mImageDisplayMenu;
};

#endif // MAINTOOLBAR_H
