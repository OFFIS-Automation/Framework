#include "RcFlagWidgetSettingsDialog.h"

#include <QBoxLayout>

RcFlagWidgetSettingsDialog::RcFlagWidgetSettingsDialog(QWidget *parent) :
    QDialog(parent, Qt::Popup)
{
    setLayout(new QVBoxLayout());
}

void RcFlagWidgetSettingsDialog::setVisible(bool visible)
{
    QDialog::setVisible(visible);
    if(!visible)
        emit hidden();
}


