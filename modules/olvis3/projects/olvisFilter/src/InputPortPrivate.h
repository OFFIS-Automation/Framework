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

#ifndef INPUTPORTPRIVATE_H
#define INPUTPORTPRIVATE_H

#include <core/InputPort.h>

class InputListPort : public InputPort
{
public:
    InputListPort(Port& parent);
    virtual bool isList() { return true; }
    virtual bool hasValue();
    virtual void setValue(const QVariant& var);
    virtual QVariant getRawValue();
    virtual void unsetChanged();
    virtual void recheck();
protected:
    QList<QVariant> mList;
    QListIterator<QVariant> mIter;
};

#endif // INPUTPORTPRIVATE_H
