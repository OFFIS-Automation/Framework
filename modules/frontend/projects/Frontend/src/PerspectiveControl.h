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

#ifndef PERSPECTIVECONTROL_H
#define PERSPECTIVECONTROL_H

#include <QObject>
#include <QMenu>
#include <QStringList>
#include <QToolBar>
#include <QActionGroup>
#include "Perspective.h"

class MainWindow;
class MasterWindow;

class PerspectiveControl : public QObject, public MainWindowInterface
{
    Q_OBJECT
public:
    explicit PerspectiveControl(MasterWindow *master, MainWindow* slave = 0, QObject *parent = 0);
    QStringList perspectives() { return mPerspectives.keys(); }
    QString currentPerspective() { return mCurrenPerspective; }
    QToolBar* toolbar() { return mToolBar; }
    void start();

    virtual void addDockWidget(Qt::DockWidgetArea area, QDockWidget * dockwidget, const QString& group);
    virtual void addDockWidget(Qt::DockWidgetArea area, QDockWidget * dockwidget, Qt::Orientation orientation, const QString& group);
    virtual void removeDockWidget(QDockWidget * dockwidget);
    virtual void addToolBar(Qt::ToolBarArea area, QToolBar * toolbar, const QString& group);
    virtual void addToolBar(QToolBar * toolbar, const QString& group);
    virtual QMenu& getMenu(QString name, QStringList subMenu = QStringList());
    virtual PerspectiveInterface& getPerspective(const QString& perspective);
    static const int cButtonWidth = 80;
signals:
    void perspectiveChanged(const QString& name);
    void perspectiveCreated(const QString& name);
    bool triggerStart();
public slots:
    void loadPerspective(const QString& name, bool saveCurrent = true);
    void savePerspective(QString name = QString());
    void resetPerspectives();
    void storePerspectives(const QString &perspective = QString());
private slots:
    void createPerspective(QString name = QString(), bool trigger = true);
    void deletePerspective();
    void perspectiveActionTriggered();
    void startDelayed();

protected:
    void addElement(QWidget* widget, const QString &group);
    void removeElement(QWidget* widget);
    void updateElements();
    void reassignShortcuts();
    MasterWindow* mMaster;
    MainWindow *mSlave;

    QMap<QString, Perspective> mPerspectives;
    QMap<QString, QAction*> mTriggers;
    QString mCurrenPerspective;
    QToolBar* mToolBar;
    QAction* mDeleteAction, *mResetAction;
    QActionGroup mActionGroup;
    QMap<QWidget*, QString> mElements;
    QMenu* mElementMenu;
};

#endif // PERSPECTIVECONTROL_H
