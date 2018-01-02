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

#include "ContourWriter.h"
#include <opencv2/imgproc.hpp>

#include <QTextStream>

REGISTER_FILTER(ContourWriter);

ContourWriter::ContourWriter()
{
    setName("ContourWriter");
    setDesc(QObject::tr("Writes a extracted contour to disc"));
    setGroup("image/contour");
    mFile.setName("fileName");
    mOffsetIn.setName("offset");
    mOffsetIn.setDesc(QObject::tr("Offset that will be added to all calculated positions"));
    mOffsetIn.setDefault(QPointF(0.0, 0.0));
    mContour.setName("contour");
    mContour.setDesc(QObject::tr("A list of the calculated outer contours"));
    addInputPort(mFile);
    addInputPort(mOffsetIn);
    addInputListPort(mContour);
}

void ContourWriter::execute()
{
    QFileInfo info = mFile;
    QFile file(info.absoluteFilePath());
    file.open(QFile::WriteOnly);
    QTextStream stream(&file);
    QPointF offset = mOffsetIn;
    while(mContour.hasValue())
    {
        QPolygonF poly = mContour.getValue();
        foreach(QPointF p, poly)
        {
            p+= offset;
            stream << QString::number(p.x()) << "," << QString::number(p.y()) << " ";
        }
        stream << "\n";
    }
}
