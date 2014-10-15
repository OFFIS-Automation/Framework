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

#include "RectInput.h"

REGISTER_FILTER(RectInput);

RectInput::RectInput()
{
    setName("RectInput");
    setDesc("Input filter for the automation.");
    setGroup("hilec/input");
    mIn.setName("input rect");
    mIn.setMode(SingleShotPortMode);
    mOut.setName("output");
    mUpdated.setName("updated");
    addInputPort(mIn);
    addOutputPort(mOut);
    addOutputPort(mUpdated);
}

void RectInput::execute()
{
    bool updated = mIn.hasValue();
    if(updated)
        mOut.send(mIn.getValue());
    mUpdated.send(updated);
}
