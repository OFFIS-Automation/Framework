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

#include "CorrectPosition.h"

REGISTER_FILTER(CorrectPosition);
CorrectPosition::CorrectPosition()
{
    setName("CorrectPosition");
    setDesc(QObject::tr("Corrects the given position by adding an offset"));
    setGroup("geometric/position");

    mIn.setName("input");
    mIn.setDesc(QObject::tr("Input point"));
    addInputPort(mIn);

    mOut.setName("output");
    mOut.setDesc(QObject::tr("Output point"));
    addOutputPort(mOut);

    mXOffset.setName("xCorrection");
    mXOffset.setDesc(QObject::tr("This value will be added to the x value of the position"));
    mXOffset.setRange(INT_MIN, INT_MAX);
    mXOffset.setDefault(0.0);
    addInputPort(mXOffset);

    mYOffset.setName("yCorrection");
    mYOffset.setDesc(QObject::tr("This value will be added to the y value of the position"));
    mYOffset.setRange(INT_MIN, INT_MAX);
    mYOffset.setDefault(0.0);
    addInputPort(mYOffset);

}

void CorrectPosition::execute()
{
    QPointF point = mIn;
    point += QPointF((double)mXOffset, (double)mYOffset);
    mOut.send(point);
}
