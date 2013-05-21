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

#include "PathCreator.h"


REGISTER_FILTER(PathCreator);

PathCreator::PathCreator()
{
    mNumber = 0;

    setName("PathCreator");
    setDesc("Creates a filepath in the given directory with the given prefix. The output file path is [dir]/[prefix][number].[extension].");
    setGroup("output");
    mDir.setName("inDirectory");
    mDir.setDesc("Directory");
    addInputPort(mDir);

    mPrefix.setName("inPrefix");
    mPrefix.setDesc("Filename prefix");
    mPrefix.setDefault("image_");
    addInputPort(mPrefix);

    mExtension.setName("inFileExtension");
    mExtension.setDesc("Filename extension");
    mExtension.setDefault("png");
    addInputPort(mExtension);

    mOut.setName("outFile");
    mOut.setDesc("The constructed filename");
    addOutputPort(mOut);
}

void PathCreator::execute()
{
    QDir dir = mDir;
    QString prefix = mPrefix;
    QString ext = mExtension;
    QString filename = prefix + QString::number(mNumber++) + "." + ext;
    mOut.send(QFileInfo(dir, filename));
}
