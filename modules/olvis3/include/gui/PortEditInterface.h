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

#ifndef PORTEDITINTERFACE_H
#define PORTEDITINTERFACE_H


#include <core/PortInfo.h>

class QString;
class QWidget;
class QVariant;
#include <QObject>

class PortEditInterface
{
public:
    class Callback {
    public:
        virtual void editFinished(const QVariant& var) = 0;
        virtual void editCanceled() = 0;
    };
    virtual ~PortEditInterface(){}
    virtual void setInfo(const PortInfo& info) = 0;
    virtual void setValue(const QVariant& variant) = 0;
    virtual QString asString() = 0;
    virtual QWidget* editWidget() = 0;
    virtual void startEdit(Callback& callback) = 0;
};

#endif // PORTEDITINTERFACE_H
