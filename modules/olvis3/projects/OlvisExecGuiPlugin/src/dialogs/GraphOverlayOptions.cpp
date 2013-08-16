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


#include "GraphOverlayOptions.h"
#include "ui_GraphOverlayOptions.h"

GraphOverlayOptions::GraphOverlayOptions(bool showHistory, bool limitedSamples, int numSamples, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphOverlayOptions)
{
    ui->setupUi(this);
    ui->numEntries->setValue(numSamples);
    ui->newEntries->setChecked(limitedSamples);
    ui->showHistory->setChecked(showHistory);


}

GraphOverlayOptions::~GraphOverlayOptions()
{
    delete ui;
}

bool GraphOverlayOptions::showHistory()
{
    return ui->showHistory->isChecked();
}

bool GraphOverlayOptions::limitedValues()
{
    return ui->newEntries->isChecked();
}

int GraphOverlayOptions::maxValues()
{
    return ui->numEntries->value();
}
