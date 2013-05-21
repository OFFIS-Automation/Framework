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

#include "VideoControlToolbar.h"
#include "ui_VideoControlToolbar.h"

#include "src/OlvisSingleton.h"

#include <QToolButton>
#include <QMenu>

VideoControlToolbar::VideoControlToolbar(QWidget *parent) :
    QToolBar(parent),
    ui(new Ui::VideoControlToolbar),
    mEditActions(this), mBoolActions(this)
{

    mBoolActions.setExclusive(false);
    ui->setupUi(this);
    mEditActions.addAction(ui->actionEditOverlays);
    ui->actionEditOverlays->setChecked(true);

    connect(&mEditActions, SIGNAL(triggered(QAction*)), this, SIGNAL(editChangeRequested(QAction*)));
    connect(&mBoolActions, SIGNAL(triggered(QAction*)), this, SIGNAL(boolChangeRequested(QAction*)));

    const OlvisInterface& model = OlvisSingleton::instance();
    connect(&model, SIGNAL(portValueChanged(int,QString,QVariant)), this, SLOT(portValueChanged(int,QString,QVariant)));
}

VideoControlToolbar::~VideoControlToolbar()
{
    clear();
    delete ui;
}

void VideoControlToolbar::portValueChanged(int filterId, const QString &portId, QVariant value)
{
    QList<QAction*> actions = mPortIds.keys(PortId(filterId, portId));
    foreach (QAction* action, actions) {
        const OlvisInterface& model = OlvisSingleton::instance();
        const PortInfo& info = model.getPortInfo(filterId, portId);
        if (info.typeName == "Boolean") {
            action->setChecked(value.toBool());
        }
        else if(info.constraints.contains("choices"))
        {
            QMenu* menu = action->menu();
            if(menu)
            {
                foreach(QAction* menuAction, menu->actions())
                {
//                    qDebug() << menuAction->data() << value << menuAction->text();
                    if(menuAction->data() == value)
                    {
                        menuAction->setChecked(true);
                        break;
                    }
                }
            }
        }
    }
}

bool VideoControlToolbar::showImageInfo()
{
    return ui->actionShowInfo->isChecked();
}

void VideoControlToolbar::addPortAction(const PortId &portId)
{
    if (mPortIds.key(portId, 0))
        return;
    const OlvisInterface& model = OlvisSingleton::instance();
    PortInfo p = model.getPortInfo(portId);

    if(p.constraints.contains("choices"))
    {
        QList<QVariant> values = p.constraints.value("choices").toList();
        QStringList names = p.constraints.value("choiceNames").toStringList();
        QAction* action = new QAction(QPixmap::fromImage(p.icon), p.desc, this);
        QMenu* menu = new QMenu(this);
        action->setMenu(menu);
        QActionGroup* group = new QActionGroup(menu);
        group->setExclusive(true);
        QVariant currentValue = model.getPortValue(portId);
        for(int i=0;i<values.size(); i++)
        {
            QVariant value = values[i];
            QString name = names.value(i, value.toString());
            QAction * item = menu->addAction(name);
            item->setData(value);
            item->setCheckable(true);
            group->addAction(item);
            if(value == currentValue)
                item->setChecked(true);
            mChoicePortIds[item] = portId;
        }
        QToolButton* button = new QToolButton(this);
        button->setDefaultAction(action);
        //button->setMenu(menu);
        button->setPopupMode(QToolButton::InstantPopup);
        addWidget(button);
//        addAction(action);
        connect(group, SIGNAL(triggered(QAction*)), SIGNAL(choiceChangeRequested(QAction*)));
        mPortIds[action] = portId;
    }
    else if (p.typeName == "Rect" || p.typeName == "Integer" || p.typeName == "Real" || p.typeName == "Point") {
        QAction* action = new QAction(QPixmap::fromImage(p.icon), p.desc, this);
        action->setCheckable(true);
        mEditActions.addAction(action);
        addAction(action);
        mPortIds[action] = portId;
    } else if (p.typeName == "Boolean") {
        QAction* action = new QAction(QPixmap::fromImage(p.icon), p.desc, this);
        action->setCheckable(true);
        action->setChecked(model.getPortValue(portId).toBool());
        insertAction(ui->actionEditOverlays, action);
        mPortIds[action] = portId;
        mBoolActions.addAction(action);
    }
    adjustSize();
}

void VideoControlToolbar::removePortAction(const PortId &portId)
{
    QAction* action = mPortIds.key(portId, 0);
    if (action) {
        removeAction(action);
        mPortIds.remove(action);
        adjustSize();
    }
}

void VideoControlToolbar::clear()
{
    QList<QAction*> actions = mPortIds.keys();
    foreach (QAction* action, actions) {
        if (action != 0) {
            removeAction(action);
            delete action;
        }
    }
    mPortIds.clear();
    ui->actionEditOverlays->setChecked(true);
    adjustSize();
}

QAction* VideoControlToolbar::currentAction()
{
    if (mEditActions.checkedAction() != ui->actionEditOverlays)
        return mEditActions.checkedAction();
    return 0;
}

PortInfo VideoControlToolbar::currentPortInfo()
{
    if (currentAction()) {
        const OlvisInterface& model = OlvisSingleton::instance();
        return model.getPortInfo(currentPortId());
    } else {
        return PortInfo();
    }
}

PortId VideoControlToolbar::portId(QAction *action)
{
    return mPortIds.value(action, mChoicePortIds.value(action));
}

PortId VideoControlToolbar::currentPortId()
{
    return mPortIds[currentAction()];
}

void VideoControlToolbar::setZoomToFit(bool value)
{
    ui->actionZoomFit->setChecked(value);
}

void VideoControlToolbar::setFlip(bool hFlip, bool vFlip)
{
    ui->actionHorizontalFlip->setChecked(hFlip);
    ui->actionVerticalFlip->setChecked(vFlip);
}

void VideoControlToolbar::on_actionClear_triggered()
{
    emit clearClicked();
}

void VideoControlToolbar::on_actionZoom100_triggered()
{
    emit zoomChanged(1.0);
}

void VideoControlToolbar::on_actionZoomFit_triggered(bool checked)
{
    emit zoomToFitChanged(checked);
}

void VideoControlToolbar::on_actionSaveImage_triggered()
{
    emit screenshotRequested();
}

void VideoControlToolbar::on_actionSaveVideo_triggered()
{
    emit videoRequested();
}

void VideoControlToolbar::on_actionHorizontalFlip_triggered(bool checked)
{
    emit horizontalFlipChanged(checked);
}

void VideoControlToolbar::on_actionVerticalFlip_triggered(bool checked)
{
    emit verticalFlipChanged(checked);
}
