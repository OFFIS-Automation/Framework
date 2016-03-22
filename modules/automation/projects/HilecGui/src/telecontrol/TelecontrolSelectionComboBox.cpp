#include "TelecontrolSelectionComboBox.h"
#include "ui_TelecontrolSelectionComboBox.h"

TelecontrolSelectionComboBox::TelecontrolSelectionComboBox(const QString &unitName, QStringList deviceNames, const QString& currentDeviceName) :
    QWidget(),
    mDeviceNames(deviceNames),
    mUnitName(unitName),
    ui(new Ui::TelecontrolSelectionComboBox)
{
    ui->setupUi(this);

    // Fill comboBox
    for(int i=0; i < deviceNames.count(); i++) {
        ui->availableDevices->addItem(deviceNames.at(i));
    }

    int indexOfCurrentHapticDeviceName = ui->availableDevices->findText(currentDeviceName);
    if(indexOfCurrentHapticDeviceName >= 0){
        ui->availableDevices->setCurrentIndex(indexOfCurrentHapticDeviceName);
    }
}

TelecontrolSelectionComboBox::~TelecontrolSelectionComboBox()
{
    delete ui;
}


void TelecontrolSelectionComboBox::on_availableDevices_currentIndexChanged(int index)
{
    QString deviceName = 0;
    if(index > 0){
        deviceName =  mDeviceNames.at(index-1);
    }
    emit telecontrolSelected(deviceName, mUnitName);
}
