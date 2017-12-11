// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
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

#include "FileEdit.h"
#include <ports/FilePort.h>

#include <QFileDialog>
#include <QLineEdit>

FileEdit::FileEdit(QWidget *parent):  AbstractPortEditWidget(parent)
{
    mLineEdit = new QLineEdit(this);
    mLineEdit->setObjectName("lineEdit");
    mLineEdit->setDisabled(true);

    QPalette *palette = new QPalette();
    palette->setColor(QPalette::Base, Qt::white);
    palette->setColor(QPalette::Text, Qt::black);
    mLineEdit->setPalette(*palette);

    mSelectFileButton = new QPushButton(tr("Select file"), this);
    mSelectFileButton->setObjectName("select_file");
    mSelectFileButton->setCheckable(false);

    ui->layout->insertWidget(0, mLineEdit);
    ui->layout->insertWidget(1, mSelectFileButton);
    QMetaObject::connectSlotsByName(this);
}

FileEdit::~FileEdit()
{
}

QString FileEdit::asString()
{
    if(mValue.isValid()){
        QFileInfo fileInfo = port::File::fromVariant(mValue);
        return fileInfo.absoluteFilePath();
    } else {
        return QString();
    }
}

void FileEdit::onStartEdit()
{
    if(mValue.isValid()){
       QFileInfo fileInfo = port::File::fromVariant(mValue);
       mLineEdit->setText(fileInfo.fileName());
       mLineEdit->setToolTip(fileInfo.absoluteFilePath());
    } else {
        mLineEdit->clear();
        mLineEdit->setToolTip("");
    }
}

QVariant FileEdit::editValue(bool &)
{
    return mValue;
}

void FileEdit::on_select_file_clicked()
{
    QString file;
    bool readMode = mInfo.constraints.value("readMode").toBool();
    QString current = port::File::fromVariant(mValue).absolutePath();
    QString filter;
    QStringList filterList = mInfo.constraints.value("filter").toStringList();
    if(!filterList.isEmpty()){
        QStringList types;
        types << tr("Supported files (%1)", "File extensions are automatically added").arg(filterList.join(" "));
        types << tr("All Files (*.*)");
        filter = types.join(";;");
    }
    if(readMode){
        file = QFileDialog::getOpenFileName(0, "Select source file", current, filter);
    } else {
        file = QFileDialog::getSaveFileName(0, "Select destination file", current, filter);
    }
    if(!file.isEmpty()){
        // Update value, submit if necessary
        QFileInfo fileInfo(file);
        mValue = port::File::variant(fileInfo);
        mLineEdit->setText(fileInfo.fileName());
        mLineEdit->setToolTip(fileInfo.absoluteFilePath());

        if(mAutoSubmit){
            editFinished(mValue);
        }
    }
}
