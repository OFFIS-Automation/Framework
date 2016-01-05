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

#include "PointInput.h"

REGISTER_FILTER(PointInput);
REGISTER_FILTER(PointListInput);

PointInput::PointInput(bool list)
{
    mAsList = list;
    if(list)
        setName("PointList");
    else
        setName("Point");
    setDesc("Input filter for the automation.");
    setGroup("automation/input");
    mIn.setName("input point");
    mIn.setMode(SingleShotPortMode);
    mOut.setName("output");
    mSingleOut.setName("singleShot");
    mSingleOut.setVisibility(ExpertPortVisibility);
    mUpdated.setName("updated");
    mUpdated.setVisibility(ExpertPortVisibility);
    if(list)
    {
        mPoly.setName("shape");
        mPoly.setDesc("the points connected to a shape");
        mPoly.setVisibility(AdvancedPortVisibility);
        addInputListPort(mIn);
        addOutputListPort(mOut);
        addOutputListPort(mSingleOut);
        addOutputPort(mPoly);
    }
    else
    {
        addInputPort(mIn);
        addOutputPort(mOut);
        addOutputPort(mSingleOut);
    }
    addOutputPort(mUpdated);
}

void PointInput::execute()
{
    bool updated = mIn.hasValue();
    if(updated)
    {
        mLast.clear();
        mLast << mIn.getValue2();
        while(mAsList && mIn.hasValue())
            mLast << mIn.getValue2();
    }
    foreach(const QPointF& p, mLast)
    {
        mOut.send(p);
        if(updated)
            mSingleOut.send(p);
    }
    if(mAsList)
        mPoly.send(QPolygonF(mLast));
    mUpdated.send(updated);
}
