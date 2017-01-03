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

#ifndef DIRECTORYEDIT_H
#define DIRECTORYEDIT_H

#include "AbstractPortEdit.h"

class DirectoryEdit : public QObject, public AbstractPortEdit
{
public:
    DirectoryEdit();
    virtual QString asString();
    virtual void onStartEdit();
    // setAutoSubmit is here for virtual reasons,
    // widget is autoSubmit by default
    virtual void setAutoSubmit(bool autoSubmit) { Q_UNUSED(autoSubmit); }
};
#endif // DIRECTORYEDIT_H
