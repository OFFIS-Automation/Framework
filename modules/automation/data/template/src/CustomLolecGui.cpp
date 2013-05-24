#include "CustomLolecGui.h"
#include "ui_CustomLolecGui.h"
#include "CustomLolec.h"

CustomLolecGui::CustomLolecGui(CustomLolec* controller, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomLolecGui),
    mController(controller)
{
    ui->setupUi(this);
    connect(mController, SIGNAL(connectStatusChanged(bool)), ui->connectButton, SLOT(setChecked(bool)));
    connect(this, SIGNAL(connectClicked()), mController, SLOT(acquireHardware()));
    connect(this, SIGNAL(disconnectClicked()), mController, SLOT(releaseHardware()));
}

CustomLolecGui::~CustomLolecGui()
{
    delete ui;
}

void CustomLolecGui::on_connectButton_clicked(bool checked)
{
    if(checked)
        emit connectClicked();
    else
        emit disconnectClicked();
}
