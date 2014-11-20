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

#include "MathFilter.h"

//REGISTER_FILTER(MathFilter);

MathFilter::MathFilter()
{
    setName("MathFilter");
    setGroup("helper");
    setDesc("Applies some fancy math to the input.");

    //mIn.setName("inValues");
    //mIn.setDesc("input values for calculation");
    //addOutputPort(mIn);

    mMode.setName("mode");
    mMode.setDesc("Calculation mode");
    mMode.addChoice(SumMode, "Sum");
    mMode.addChoice(MeanMode, "Mean");
    mMode.setDefault(SumMode);
    mMode.setMode(SingleShotPortMode);
    addInputPort(mMode);

    mOut.setName("result");
    mOut.setDesc("Calculation result");
    addOutputPort(mOut);
}

void MathFilter::execute()
{

    static int testDJ = 0;
    testDJ++;

    mOut.send(testDJ);
    /*int mode = mMode;

    if (mode == SumMode || mode == MeanMode)
    {
        foreach (int value, inputs)
            result += value;

        if (mode == MeanMode)
        {
            if(inputs.count() > 0)
                result = result / inputs.count();
        }
    }
    mOut.send(result);*/
}
