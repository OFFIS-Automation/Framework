#include "CustomRcUnitGui.h"
#include "ui_CustomRcUnitGui.h"
#include "CustomRcUnit.h"

CustomRcUnitGui::CustomRcUnitGui(CustomRcUnit* controller, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomRcUnitGui),
    mController(controller)
{
    ui->setupUi(this);
    connect(mController, SIGNAL(connectStatusChanged(bool)), ui->connectButton, SLOT(setChecked(bool)));
    connect(this, SIGNAL(connectClicked()), mController, SLOT(acquireHardware()));
    connect(this, SIGNAL(disconnectClicked()), mController, SLOT(releaseHardware()));
}

CustomRcUnitGui::~CustomRcUnitGui()
{
    delete ui;
}

void CustomRcUnitGui::on_connectButton_clicked(bool checked)
{
    if(checked)
        emit connectClicked();
    else
        emit disconnectClicked();
}
