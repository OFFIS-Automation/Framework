#include "HapticSelectionComboBox.h"
#include "ui_HapticSelectionComboBox.h"

HapticSelectionComboBox::HapticSelectionComboBox(const QString &unitName, QStringList hapticInterfaces, const QString& currentHapticInterfaceName) :
    QWidget(),
    mHapticInterfaces(hapticInterfaces),
    mUnitName(unitName),
    ui(new Ui::HapticSelectionComboBox)
{
    ui->setupUi(this);

    // Fill comboBox
    for(int i=0; i < mHapticInterfaces.count(); i++) {
        ui->availableInterfaces->addItem(mHapticInterfaces.at(i));
    }

    int indexOfCurrentHapticInterfaceName = ui->availableInterfaces->findText(currentHapticInterfaceName);
    if(indexOfCurrentHapticInterfaceName >= 0){
        ui->availableInterfaces->setCurrentIndex(indexOfCurrentHapticInterfaceName);
    }
}

HapticSelectionComboBox::~HapticSelectionComboBox()
{
    delete ui;
}

void HapticSelectionComboBox::on_availableInterfaces_currentIndexChanged(int index)
{
    QString hapticInterfaceName = 0;
    if(index > 0){
        hapticInterfaceName =  mHapticInterfaces.at(index-1);
    }
    emit hapticSelected(mUnitName, hapticInterfaceName);
}
