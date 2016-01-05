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

#include "ScriptUi.h"
#include "ui_ScriptUi.h"
#include "ScriptInput.h"
#include "ScriptProgress.h"
#include "ScriptInfoPanel.h"

#include "../HilecSingleton.h"

#include <core/UserRequest.h>


ScriptUi::ScriptUi(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ScriptUi)
{
    qRegisterMetaType<UserRequest>("UserRequest");
    ui->setupUi(this);
    ui->informationBox->hide();
    connect(HilecSingleton::hilec(), SIGNAL(userInputRequested(UserRequest)), SLOT(handleUserRequest(UserRequest)));
    connect(HilecSingleton::hilec(), SIGNAL(userInputAborted(int)), SLOT(abortUserRequest(int)));
    connect(HilecSingleton::hilec(), SIGNAL(newProgressBar(int,QString, int)), SLOT(onNewPrgoressBar(int,QString, int)));
    connect(HilecSingleton::hilec(), SIGNAL(progressBarUpdated(int,int)), SLOT(onProgressBarUpdated(int,int)));
    connect(HilecSingleton::hilec(), SIGNAL(progressBarRemoved(int)), SLOT(onProgressBarRemoved(int)));
    connect(HilecSingleton::hilec(), SIGNAL(newInfoPanel(int,QString,QStringList)), SLOT(onNewInfoPanel(int,QString,QStringList)));
    connect(HilecSingleton::hilec(), SIGNAL(infoPanelUpdated(int,QStringList)), SLOT(onInfoPanelUpdated(int,QStringList)));
    connect(HilecSingleton::hilec(), SIGNAL(infoPanelRemoved(int)), SLOT(onInfoPanelRemoved(int)));
    connect(HilecSingleton::hilec(), SIGNAL(infoCleared()), ui->information, SLOT(clear()));
    connect(HilecSingleton::hilec(), SIGNAL(infoCleared()), ui->informationBox, SLOT(hide()));
    connect(HilecSingleton::hilec(), SIGNAL(infoAppended(QString)),  ui->information, SLOT(append(QString)));
    connect(HilecSingleton::hilec(), SIGNAL(infoAppended(QString)),  ui->informationBox, SLOT(show()));

    connect(HilecSingleton::hilec(), SIGNAL(scriptExecutionFinished()), SLOT(onScriptFinished()));
    //connect(HilecSingleton::hilec(), SIGNAL(), SLOT());
    connect(this, SIGNAL(newUserInput(int,int,QList<QVariant>)), HilecSingleton::hilec(), SLOT(userInput(int,int, const QList<QVariant>&)));
}

ScriptUi::~ScriptUi()
{
    delete ui;
}

void ScriptUi::handleUserRequest(const UserRequest &r)
{
    ScriptInput* input = new ScriptInput(r);
    ui->inputLayout->addWidget(input);
    mInputs[r.uid] = input;
    connect(input, SIGNAL(newUserInput(int,int,QList<QVariant>)), SLOT(onUserInput(int,int,QList<QVariant>)));
    if(!isVisible())
        show();
}

void ScriptUi::abortUserRequest(int uid)
{
    QWidget* w = mInputs.take(uid);
    ui->inputLayout->removeWidget(w);
    delete w;
}

void ScriptUi::onUserInput(int uid, int buttonId, const QList<QVariant> &data)
{
    abortUserRequest(uid);
    emit newUserInput(uid, buttonId, data);
}

void ScriptUi::onNewPrgoressBar(int id, const QString &name, int maximum)
{
    ScriptProgress* progress = new ScriptProgress(name, maximum);
    ui->progressLayout->addWidget(progress);
    if(mProgressBars.contains(id))
        onProgressBarRemoved(id);
    mProgressBars[id] = progress;
}

void ScriptUi::onProgressBarUpdated(int id, int progress)
{
    if(!mProgressBars.contains(id))
        return;
    mProgressBars[id]->setProgress(progress);
}

void ScriptUi::onProgressBarRemoved(int id)
{
    if(!mProgressBars.contains(id))
        return;
    ScriptProgress* progress = mProgressBars.take(id);
    ui->progressLayout->removeWidget(progress);
    delete progress;
}

void ScriptUi::onNewInfoPanel(int id, const QString &title, const QStringList &names)
{
    ScriptInfoPanel* info = new ScriptInfoPanel(title, names);
    ui->infoLayout->addWidget(info);
    if(mInfoPanels.contains(id))
        onInfoPanelRemoved(id);
    mInfoPanels[id] = info;
}

void ScriptUi::onInfoPanelUpdated(int id, const QStringList &values)
{
    if(!mInfoPanels.contains(id))
        return;
    mInfoPanels[id]->updateValues(values);
}

void ScriptUi::onInfoPanelRemoved(int id)
{
    if(!mInfoPanels.contains(id))
        return;
    ScriptInfoPanel* info= mInfoPanels.take(id);
    ui->infoLayout->removeWidget(info);
    delete info;
}

void ScriptUi::onScriptFinished()
{
    foreach(int key, mInfoPanels.keys())
        onInfoPanelRemoved(key);
    foreach(int key, mProgressBars.keys())
        onProgressBarRemoved(key);
    ui->information->clear();
}

