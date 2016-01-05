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

#ifndef RECTPORT_H
#define RECTPORT_H
#include "DefaultPortsGlobal.h"
#include <QList>
#include <QRect>
#include <opencv2/core/core.hpp>



namespace port
{
    class DEFAULTPORTSSHARED_EXPORT Rect : public OverlayPort
    {
    public:
        static QRectF qrect(const cv::Rect& r);
        static cv::Rect rect(const QRectF& r);
        QVariant variant(const QRect& r);
        inline void setDefault(const cv::Rect& value) { setDefault(qrect(value)); }
        void setDefault(const QRectF& rect);
        inline void setMax(const cv::Rect& value) { setMax(qrect(value)); }
        void setMax(const QRectF& rect);
        void setMin(const QSizeF& size);

        inline void setWidthOnlyEven(bool roundDown = true) { setWidthConstraint(2, 0, roundDown); }
        inline void setWidthOnlyOdd(bool roundDown = true) { setWidthConstraint(2, 1, roundDown); }

        inline void setHeightOnlyEven(bool roundDown = true) { setHeightConstraint(2, 0, roundDown); }
        inline void setHeighthOnlyOdd(bool roundDown = true) { setHeightConstraint(2, 1, roundDown); }

        void setWidthConstraint(int divisor, int result = 0, bool roundDown = true);
        void setHeightConstraint(int divisor, int result = 0, bool roundDown = true);

        void setSquareOnly(bool onlySquareShapeAllowed);
    protected:
        Rect();

    protected:
        QString toString(const QVariant &var) const;
        QVariant fromString(const QString &str) const;
        QVariant constrainedValue(const QVariant &var) const;
        QRectF constrainDimensions(QRectF r) const;
        QRectF mMax;
        QSizeF mMin;
        bool mSquareOnly;
        int mWDiv, mWRes, mHDiv, mHRes, mWStep, mHStep;
    };
}

namespace out
{
    class DEFAULTPORTSSHARED_EXPORT Rect : public port::Rect
    {
    public:
        inline void send(const cv::Rect& value) { send(qrect(value)); }
        void send(const QRectF& rect);
    };
}

namespace in
{
    class DEFAULTPORTSSHARED_EXPORT Rect : public port::Rect
    {
    public:
        cv::Rect getValue();
        QRectF getValue2();
        inline operator cv::Rect() { return getValue(); }
        inline operator QRectF() { return getValue2(); }
        inline operator QRect() { return getValue2().toRect(); }
    };
}

#endif // RECTPORT_H
