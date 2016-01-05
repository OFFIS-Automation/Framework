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

#ifndef POSEPORT_H
#define POSEPORT_H

#include "DefaultPortsGlobal.h"
#include "Pose2d.h"

namespace port
{
    class DEFAULTPORTSSHARED_EXPORT Pose2d : public Port
    {
    public:
        static QVariant variant(const ::Pose2d& pose);
        static ::Pose2d fromVariant(const QVariant& var);
        void setDefault(const ::Pose2d& Point);
    protected:
        Pose2d();
        QString toString(const QVariant &var) const;
        QVariant fromString(const QString &str) const;
        QVariant constrainedValue(const QVariant &var) const { return var; }
        QVariant toSimpleType(const QVariant& var) const;
    };
}

namespace out
{
    class DEFAULTPORTSSHARED_EXPORT Pose2d : public port::Pose2d
    {
    public:
        void send(const ::Pose2d& value);
    };
}

namespace in
{
    class DEFAULTPORTSSHARED_EXPORT Pose2d : public port::Pose2d
    {
    public:
        ::Pose2d getValue();
        inline operator ::Pose2d() { return getValue(); }
    };
}


#endif // POSEPORT_H
