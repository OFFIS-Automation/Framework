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

#ifndef ABSTRACTPORTEDIT_H
#define ABSTRACTPORTEDIT_H

#include <gui/PortEditInterface.h>
#include <core/PortInfo.h>

class AbstractPortEdit : public PortEditInterface
{
public:
    AbstractPortEdit();
    virtual void setInfo(const PortInfo &info) { mInfo = info; }
    virtual void setValue(const QVariant &variant) { mValue = variant; }
    virtual void setAutoSubmit(bool autoSubmit) = 0;
    virtual QWidget* editWidget() { return 0; }
    virtual void startEdit(Callback& callback);
    virtual void onStartEdit() = 0;

protected:
    void editFinished(const QVariant& value);
    void editCanceled();

    PortInfo mInfo;
    QVariant mValue;
    bool mAutoSubmit = false;

private:
    Callback* mCallback;

};

#endif // ABSTRACTPORTEDIT_H
