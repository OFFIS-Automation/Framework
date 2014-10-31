#include "HapticSelectionComboBox.h"
#include "ui_HapticSelectionComboBox.h"

HapticSelectionComboBox::HapticSelectionComboBox(const QString &unitName, QStringList mHapticDevices, const QString& currentHapticDeviceName) :
    QWidget(),
    mHapticDevices(mHapticDevices),
    mUnitName(unitName),
    ui(new Ui::HapticSelectionComboBox)
{
    ui->setupUi(this);

    // Fill comboBox
    for(int i=0; i < mHapticDevices.count(); i++) {
        ui->availableInterfaces->addItem(mHapticDevices.at(i));
    }

    int indexOfCurrentHapticDeviceName = ui->availableInterfaces->findText(currentHapticDeviceName);
    if(indexOfCurrentHapticDeviceName >= 0){
        ui->availableInterfaces->setCurrentIndex(indexOfCurrentHapticDeviceName);
    }
}

HapticSelectionComboBox::~HapticSelectionComboBox()
{
    delete ui;
}

void HapticSelectionComboBox::on_availableInterfaces_currentIndexChanged(int index)
{
    QString hapticDeviceName = 0;
    if(index > 0){
        hapticDeviceName =  mHapticDevices.at(index-1);
    }
    emit hapticSelected(mUnitName, hapticDeviceName);
}
