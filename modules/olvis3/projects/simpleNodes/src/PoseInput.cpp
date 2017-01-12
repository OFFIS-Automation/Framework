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

#include "PoseInput.h"

REGISTER_FILTER(PoseInput);

PoseInput::PoseInput()
{
    setName(tr("Pose"));
    setDesc(QObject::tr("Pose input filter for the automation"));
    setGroup("automation/input");

    mIn.setName("input pose");
    mIn.setMode(SingleShotPortMode);

    mOut.setName("output pose");

    mUpdated.setName("updated");
    mUpdated.setVisibility(ExpertPortVisibility);

    addInputPort(mIn);
    addOutputPort(mOut);
    addOutputPort(mUpdated);
}

void PoseInput::execute()
{
    bool updated = mIn.hasValue();
    if(updated){
       mOut.send(mIn.getValue());
    }
    mUpdated.send(updated);
}
