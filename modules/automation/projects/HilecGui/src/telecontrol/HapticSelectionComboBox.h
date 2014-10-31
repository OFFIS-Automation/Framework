#ifndef HAPTICSELECTIOCOMBOBOX_H
#define HAPTICSELECTIOCOMBOBOX_H

#include <QWidget>
#include "telecontrol/HapticInterface.h"

namespace Ui {
class HapticSelectionComboBox;
}

class HapticSelectionComboBox : public QWidget
{
    Q_OBJECT
public:
    explicit HapticSelectionComboBox(const QString& unitName, QStringList hapticInterfaces, const QString &currentHapticDeviceName);
    ~HapticSelectionComboBox();

signals:
    void hapticSelected(const QString& unitName, const QString& hapticInterfaceName);

private slots:
    void on_availableInterfaces_currentIndexChanged(int index);

private:
    QStringList mHapticDevices;
    Ui::HapticSelectionComboBox *ui;
    QString mUnitName;
};

#endif // HAPTICSELECTIOCOMBOBOX_H
