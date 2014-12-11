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

#ifndef PORT_H
#define PORT_H

#include <QVariant>
#include <QtGui/QPixmap>
#include "FilterBaseGlobal.h"
#include "InputPortMode.h"

class PortData;
class Filter;
struct PortInfo;

class FILTERBASESHARED_EXPORT Port
{
public:
    void setName(const QString& name);
    void setDesc(const QString& desc);
    void setIcon(const QImage &icon);

    /**
     * sets a mode for this port. The Choices are:
     *  RegularPortMode: default bahavior, port must always have a value
     *  OptionalPortMode: port may or may not have a value. The filter must check with hasValue()
     *  SingleShotPortMode: port may or may not have a value. IF a value is assigned, it is only valid for one execution of the
     *      filter. After the execution, it is automatically reset to NoValue
     */
    void setMode(InputPortMode mode);

    /**
     * Returns whether this port holds a valid value.
     * For default ports that are not lists, the return value is always true
     *
     * For list ports, this commands returns true if the list holds another value.
     * You can iterate through the input list using while(hasValue()) getValue();
     * getValue returns one element of the list and increments the list id
    */
    bool hasValue();

    /**
     * Returns whether the port was updated since the last execution
    */
    bool isUpdated();

    /**
     * Returns whether the port was update and holds a new value
     * that is distinct from the previous value
     */
    bool hasChanged();


    PortInfo getInfo() const;
    const QString& name() const;
    InputPortMode mode() const;
    // port specific methods
    virtual QString toString(const QVariant& var) const = 0;
    virtual QVariant fromString(const QString& str) const = 0;
    virtual QVariant constrainedValue(const QVariant& var) const = 0;
    virtual QVariant toSimpleType(const QVariant& var) const{ return var; }

protected:
    // methods for derived port specializations
    Port(int id, const QString& name, bool isMainType = false);
    void setConstraint(const QByteArray& key, const QVariant& value);
    void addChoiceRaw(const QVariant& value, const QString& name);
    // input port methods for ports
    QVariant getRawValue();
    QVariant setDefaultValueRaw(const QVariant& var);
    void addCompatiblePort(int typeId);
    void addCompatiblePort(int typeId, const QString& warningMsg);
    void recheck();
    // output methods for ports
    void sendRaw(const QVariant& value);

protected:
    PortData* d;
    friend class Filter;
    friend class InputPort;
    friend class OutputPort;
};

#endif // PORT_H
