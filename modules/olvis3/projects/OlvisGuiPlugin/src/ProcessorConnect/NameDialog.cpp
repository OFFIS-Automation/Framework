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

#include "NameDialog.h"

NameDialog::NameDialog(QString name, QWidget *parent) :
    QDialog(parent)
{
    QLabel *nameLabel = new QLabel(tr("Processor name:"));
    lineEdit = new QLineEdit;
    lineEdit->setText(name);

    okButton = new QPushButton(tr("&Ok"));
    name = "";

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(nameLabel);
    layout->addWidget(lineEdit);
    layout->addWidget(okButton);

    setLayout(layout);
    setWindowTitle(tr("Change processor name"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
}

void NameDialog::buttonClicked()
{
    QString text = lineEdit->text();

    if (text.isEmpty()) {
        QMessageBox::information(this, tr("Empty Field"),
            tr("Please enter a name."));
        return;
    } else {
        name = text;
        lineEdit->clear();
        hide();
    }
}

QString NameDialog::getName()
{
    return name;
}
