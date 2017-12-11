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

#include <ports/RealPort.h>
#include <QStringList>
#include <core/PortInfo.h>

#include <cfloat>

port::Real::Real() : OverlayPort(QMetaType::Double, "Real")
{
    setIcon(QImage(":/defaultPorts/images/number.png"));
    setRange(-DBL_MAX, DBL_MAX);
}

void port::Real::setDefault(double value)
{
    setDefaultValueRaw(value);
}

void port::Real::setRange(double min, double max)
{
    mMin = min;
    mMax = max;
    setConstraint("min", min);
    setConstraint("max", max);
}

void port::Real::setDecimals(int decimals)
{
    mDecimals = decimals;
    setConstraint("decimals", decimals);
}

void port::Real::addChoice(double value)
{
    addChoice(value, QString::number(value));
}

void port::Real::addChoice(double value, const QString &name)
{
    mKeys.append(value);
    addChoiceRaw(value, name);
}

QString port::Real::toString(const QVariant& var) const
{
    return QString::number(var.toDouble());
}

QVariant port::Real::fromString(const QString& str) const
{
    return QVariant(str.toDouble());
}

QVariant port::Real::constrainedValue(const QVariant &var) const
{
    double val = var.toDouble();
    val = qMax(val,mMin);
    val = qMin(val, mMax);
    if(!mKeys.isEmpty() && !mKeys.contains(val))
    {
        double minDiff = DBL_MAX;
        double v2 = val;
        foreach(double v, mKeys)
        {
            double diff = abs(v-v2);
            if(diff < minDiff)
            {
                val = v;
                minDiff = diff;
            }
        }
    }
    return QVariant(val);
}

double in::Real::getValue()
{
    return getRawValue().toDouble();
}

void in::Real::setInputSensitivity(double sensitivity)
{
    setConstraint("sensitivity", sensitivity);
}

void out::Real::send(double value)
{
    sendRaw(value);
}
