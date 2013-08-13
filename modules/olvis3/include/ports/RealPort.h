// OFFIS Automation Framework
// Copyright (C) 2013 OFFIS e.V.
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

#ifndef REALPORT_H
#define REALPORT_H

#include "DefaultPortsGlobal.h"
#include <QList>

namespace port
{
    class DEFAULTPORTSSHARED_EXPORT Real : public OverlayPort
    {
    public:
        void setDefault(double value);
        void setRange(double min, double max);
        void addChoice(double value);
        void addChoice(double value, const QString& name);
    protected:
        Real();
        QString toString(const QVariant &var) const;
        QVariant fromString(const QString &str) const;
        QVariant constrainedValue(const QVariant &var) const;
                double mMin, mMax;
        QList<double> mKeys;
    };
}

namespace out
{
    class DEFAULTPORTSSHARED_EXPORT Real : public port::Real
    {
    public:
        void send(double value);
    };


}

namespace in
{
    class DEFAULTPORTSSHARED_EXPORT Real : public port::Real
    {
    public:
        double getValue();
        inline operator double() { return getValue(); }
    };
}

#endif // REALPORT_H
