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

#ifndef INTEGERPORT_H
#define INTEGERPORT_H

#include "DefaultPortsGlobal.h"
#include <QList>

typedef QList<int> IntList;
namespace port
{
    class DEFAULTPORTSSHARED_EXPORT Integer : public OverlayPort
    {
    public:
        void setDefault(int value);
        void setRange(int min, int max);
        void addChoice(int value);
        void addChoice(int value, const QString& name);
        inline void setOnlyEven(bool roundDown = true) { setDivConstraint(2, 0, roundDown); }
        inline void setOnlyOdd(bool roundDown = true) { setDivConstraint(2, 1, roundDown); }
        void setDivConstraint(int divisor, int result = 0, bool roundDown = true);

    protected:
        Integer();
        QString toString(const QVariant &var) const;
        QVariant fromString(const QString &str) const;
        QVariant constrainedValue(const QVariant &var) const;
        int mMin, mMax, mDiv, mDivRes, mDivStep;
        QList<int> mKeys;
    };
}

namespace out
{
    class DEFAULTPORTSSHARED_EXPORT Integer : public port::Integer
    {
    public:
        void send(int value);
    };
}

namespace in
{
    class DEFAULTPORTSSHARED_EXPORT Integer : public port::Integer
    {
    public:
        void setInputSensitivity(double sensitivity = 1.0);
        int getValue();
        inline operator int() { return getValue(); }
    };
}

#endif // INTEGERPORT_H
