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

#include <QFontMetrics>
#include <QCheckBox>
#include <QDialog>
#include <QDebug>

#include "RcUnitFlagWidget.h"
#include "ui_RcUnitFlagWidget.h"

#include "../HilecSingleton.h"

RcUnitFlagWidget::RcUnitFlagWidget(const RcUnitHelp &help) :
    ui(new Ui::RcUnitFlagWidget),
    mDialog(0)
{
    ui->setupUi(this);
    mHelp = help;
    ui->groupBox->setTitle(help.unitName);
    if(help.type != RobotRcUnitType){
        ui->stopButton->hide();
    } else {
        ui->stopButton->setEnabled(help.hwConnected);
    }
    if(help.type == BaseRcUnitType){
        ui->connectButton->hide();
    } else {
        ui->connectButton->setChecked(help.hwConnected);
    }
    foreach(const RcFlagDefinition& def, help.flags){
        QLineEdit* edit = new QLineEdit();
        edit->setMinimumWidth(50);
        edit->setAlignment(Qt::AlignRight);
        edit->setReadOnly(true);
        ui->formLayout->addRow(def.name, edit);
        mLineEdits << edit;
    }
    connect(this, SIGNAL(acquire(QString)), HilecSingleton::hilec(), SLOT(callRcUnitAcquire(QString)),Qt::QueuedConnection);
    connect(this, SIGNAL(release(QString)), HilecSingleton::hilec(), SLOT(callRcUnitRelease(QString)),Qt::QueuedConnection);
    connect(this, SIGNAL(stop(QString)), HilecSingleton::hilec(), SLOT(callRcUnitStop(QString)),Qt::QueuedConnection);

    QWidget* settingsWidget = HilecSingleton::hilec()->createRcUnitWidget(help.unitName);
    if(settingsWidget){
        mDialog = new QDialog(this, Qt::Popup);
        connect(mDialog, SIGNAL(finished(int)), this, SLOT(on_hideSettingsWidget()));
        mDialog->setLayout(new QHBoxLayout());
        mDialog->layout()->addWidget(settingsWidget);
        mDialog->hide();
    } else {
        ui->settingsButton->hide();
    }
}

RcUnitFlagWidget::~RcUnitFlagWidget()
{
    delete ui;
}

void RcUnitFlagWidget::updateFlags(const QVariantList &flags)
{
    for(int i=0;i<mLineEdits.size(); i++)
    {
        QVariant data = flags.value(i);
        RcFlagDefinition def = mHelp.flags.value(i);
        QLineEdit* line = mLineEdits[i];
        QString content;
        if(data.type() == QVariant::Bool){
            content = data.toString();
        } else {
            if(data.canConvert(QVariant::Double)){
                content = QString::number(data.toDouble(), 'f', def.decimalPlaces);
            }else {
                content = data.toString();
            }
        }

        if(!def.unit.isEmpty()){
            content += " " + def.unit;
        }
        line->setText(content);
    }
}

void RcUnitFlagWidget::unitStatusChanged(bool acquired)
{
    ui->connectButton->setChecked(acquired);
    ui->stopButton->setEnabled(acquired);
    ui->connectButton->setEnabled(true);
}

void RcUnitFlagWidget::on_connectButton_clicked(bool checked)
{
    ui->connectButton->setEnabled(false);
    if(checked){
        emit acquire(mHelp.unitName);
    } else {
        emit release(mHelp.unitName);
    }
}

void RcUnitFlagWidget::on_stopButton_clicked()
{
    emit stop(mHelp.unitName);
}

void RcUnitFlagWidget::on_groupBox_clicked(bool checked)
{
    ui->contentWidget->setVisible(checked);
    if(!checked){
        mGeometry = saveGeometry();
        QFontMetrics fontMetrics(ui->groupBox->font());
        float titleWidth = fontMetrics.width(ui->groupBox->title());
        float checkboxWidth = 20.0;
        setMaximumWidth(titleWidth + checkboxWidth);
    } else {
        setMaximumWidth(INT_MAX);
        restoreGeometry(mGeometry);
    }

}

void RcUnitFlagWidget::on_settingsButton_clicked(bool checked)
{
    qDebug() << "a";
    if(mDialog){
        if(checked){
            QPoint pos = ui->settingsButton->mapToGlobal(QPoint(0,ui->settingsButton->height()));
            QSize size = mDialog->size();
            mDialog->setGeometry(QRect(pos, size));
            mDialog->show();
        }
    }
    qDebug() << "b";
}

void RcUnitFlagWidget::on_hideSettingsWidget()
{
    qDebug() << "c";
    QMutexLocker waitLock(&mWaitMutex);
    mWait.wait(&mWaitMutex, 100);
    ui->settingsButton->setChecked(false);
    qDebug() << "d";
}
