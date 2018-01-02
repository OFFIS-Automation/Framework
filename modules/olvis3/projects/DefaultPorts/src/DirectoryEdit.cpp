// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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

#include "DirectoryEdit.h"
#include <ports/DirectoryPort.h>
#include <QFileDialog>

DirectoryEdit::DirectoryEdit()
{
}

QString DirectoryEdit::asString()
{
    if(mValue.isValid())
        return port::Directory::fromVariant(mValue).absolutePath();
    else
        return QString();
}

void DirectoryEdit::onStartEdit()
{
    QString file;
    QString current =  asString();
    file = QFileDialog::getExistingDirectory(0, tr("Select directory"), current);
    if(file.isEmpty())
        editCanceled();
    else
        editFinished( port::Directory::variant(QDir(file)));
}
