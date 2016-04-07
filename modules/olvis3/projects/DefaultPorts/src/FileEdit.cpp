// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

FileEdit::FileEdit()
{
}


QString FileEdit::asString()
{
    if(mValue.isValid())
        return port::File::fromVariant(mValue).absoluteFilePath();
    else
        return QString();
}

void FileEdit::onStartEdit()
{
    QString file;
    bool readMode = mInfo.constraints.value("readMode").toBool();
    QString current = port::File::fromVariant(mValue).absolutePath();
    QString filter;
    QStringList filterList = mInfo.constraints.value("filter").toStringList();
    if(!filterList.isEmpty())
    {
        QStringList types;
        types << tr("Supported files (%1)", "extensions are automatically added").arg(filterList.join(" "));
        types << tr("All Files (*.*)");
        filter = types.join(";;");
    }
    if(readMode)
        file = QFileDialog::getOpenFileName(0, "Select file to read from", current, filter);
    else
        file = QFileDialog::getSaveFileName(0, "Select file to write to", current, filter);
    if(file.isEmpty())
        editCanceled();
    else
        editFinished( port::File::variant(QFileInfo(file)));
}
