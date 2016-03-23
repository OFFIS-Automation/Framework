#ifndef HAPTICSELECTIOCOMBOBOX_H
#define HAPTICSELECTIOCOMBOBOX_H

#include <QWidget>

namespace Ui {
class TelecontrolSelectionComboBox;
}

class TelecontrolSelectionComboBox : public QWidget
{
    Q_OBJECT
public:
    explicit TelecontrolSelectionComboBox(const QString& unitName, QStringList deviceNames, const QString &currentDeviceName);
    ~TelecontrolSelectionComboBox();

signals:
    void telecontrolSelected(const QString& deviceName, const QString& unitName);

private slots:
    void on_availableDevices_currentIndexChanged(int index);

private:
    Ui::TelecontrolSelectionComboBox *ui;
    QStringList mDeviceNames;
    QString mUnitName;
};

#endif // HAPTICSELECTIOCOMBOBOX_H
