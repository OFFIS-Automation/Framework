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

#include "PathCreator.h"


REGISTER_FILTER(PathCreator);

PathCreator::PathCreator()
{
    mNumber = 0;

    setName("PathCreator");
    setDesc(QObject::tr("Creates a file path in the given directory with the given prefix. The output file path is [dir]/[prefix][number].[extension]"));
    setGroup("output");

    mDir.setName("directory");
    addInputPort(mDir);

    mPrefix.setName("prefix");
    mPrefix.setDesc(QObject::tr("File name prefix"));
    mPrefix.setDefault("image_");
    addInputPort(mPrefix);

    mExtension.setName("fileExtension");
    mExtension.setDesc(QObject::tr("File name extension"));
    mExtension.setDefault("png");
    addInputPort(mExtension);

    mFileName.setName("fileName");
    mFileName.setDesc(QObject::tr("The file name as string without the directory"));
    mFileName.setVisibility(AdvancedPortVisibility);
    addOutputPort(mFileName);

    mFileId.setName("fileId");
    mFileId.setDesc(QObject::tr("The assigned number"));
    mFileId.setVisibility(AdvancedPortVisibility);
    addOutputPort(mFileId);

    mOut.setName("filePath");
    mOut.setDesc(QObject::tr("The constructed file name"));
    addOutputPort(mOut);
}

void PathCreator::execute()
{
    QDir dir = mDir;
    QString prefix = mPrefix;
    QString ext = mExtension;
    mFileId.send(mNumber);
    QString filename = prefix + QString::number(mNumber++) + "." + ext;
    mFileName.send(filename);
    mOut.send(QFileInfo(dir, filename));
}
