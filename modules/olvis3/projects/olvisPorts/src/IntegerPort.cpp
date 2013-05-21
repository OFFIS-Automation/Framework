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

#include <ports/IntegerPort.h>
#include <QStringList>
#include <core/PortInfo.h>


port::Integer::Integer() : Port(QMetaType::Int, "Integer")
{
    setIcon(QImage(":/defaultPorts/images/number.png"));
    setRange(INT_MIN, INT_MAX);
    setDivConstraint(0);
}

void port::Integer::setDefault(int value)
{
    setDefaultValueRaw(value);
}

void port::Integer::setRange(int min, int max)
{
    mMin = min;
    mMax = max;
    setConstraint("min", min);
    setConstraint("max", max);
}

void port::Integer::setDivConstraint(int divisor, int result, bool roundDown)
{
    setConstraint("div", divisor);
    setConstraint("divres", result);
    mDiv = divisor;
    mDivRes = result;
    if(roundDown)
        mDivStep = 0;
    else
        mDivStep = mDiv;
}

void port::Integer::addChoice(int value)
{
    addChoice(value, QString::number(value));
}

void port::Integer::addChoice(int value, const QString &name)
{
    mKeys.append(value);
    addChoiceRaw(value, name);
}

QString port::Integer::toString(const QVariant& var) const
{
    return QString::number(var.toInt());
}

QVariant port::Integer::fromString(const QString& str) const
{
    return QVariant(str.toInt());
}

QVariant port::Integer::constrainedValue(const QVariant &var) const
{
    int val = var.toInt();
    val = qMax(val,mMin);
    val = qMin(val, mMax);
    if(mDiv != 0 && val % mDiv != mDivRes)
        val = val - (val % mDiv) + mDivStep + mDivRes;

    if(!mKeys.isEmpty() && !mKeys.contains(val))
    {
        int minDiff = INT_MAX;
        int v2 = val;
        foreach(int v, mKeys)
        {
            int diff = abs(v-v2);
            if(diff < minDiff)
            {
                val = v;
                minDiff = diff;
            }
        }
    }
    return QVariant(val);
}

int in::Integer::getValue()
{
    return getRawValue().toInt();
}


void out::Integer::send(int value)
{
    sendRaw(value);
}
