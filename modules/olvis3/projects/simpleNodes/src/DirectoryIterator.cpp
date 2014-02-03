// OFFIS Automation Framework
// Copyright (C) 2013-2014 OFFIS e.V.
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

#include "DirectoryIterator.h"


REGISTER_FILTER(DirectoryIterator);

DirectoryIterator::DirectoryIterator()
    : mIter(mFileList)
{
    setName("DirectoryIterator");
    setDesc("Iterates through a directory and outputs all files that fit the filter.");
    setGroup("input");
    mOut.setName("file");
    mOut.setDesc("file name");
    addOutputPort(mOut);
    mFilter.setName("nameFilter");
    mFilter.setMode(OptionalPortMode);
    mFilter.setDesc("File extension filter, seperated by space. example: *.png *.jpg *.xpm");
    addInputPort(mFilter);
    mIn.setName("dir");
    mIn.setDesc("directory to iterate");
    addInputPort(mIn);

    mRepeat.setName("repeat");
    mRepeat.setDefault(false);
    mRepeat.setDesc("repeat from beginning of the list if finished");
    mRepeat.setIcon(QImage(":/SimpleNodes/repeat.png"));
    addInputPort(mRepeat);
}

void DirectoryIterator::start()
{
    mDir = QDir();
}

void DirectoryIterator::execute()
{
    QDir dir = mIn;
    if(!mIter.hasNext() && mRepeat)
        mDir = QDir();
    if(dir != mDir || (mFilter.hasValue() && mFilter.getValue() != mCurrentFilter))
    {
        mDir = dir;
        mCurrentFilter = mFilter;
        QDir::Filters filters = QDir::NoDotAndDotDot | QDir::Files;
        if(mFilter.hasValue())
        {
            QStringList nameFilters = mFilter.getValue().split(" ");
            mFileList = mDir.entryInfoList(nameFilters, filters, QDir::Name | QDir::IgnoreCase);
        }
        else
            mFileList = mDir.entryInfoList(filters, QDir::Name | QDir::IgnoreCase);

        //mFileList = mDir.entryInfoList(QStringList("*.png") << "*.jpg" << "*.bmp", QDir::NoDotAndDotDot | QDir::Files, QDir::Name | QDir::IgnoreCase);
        mIter = mFileList;
    }
    if(mIter.hasNext())
    {
        mOut.send(mIter.next());
    }
}
