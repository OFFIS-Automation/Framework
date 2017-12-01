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

#ifndef INPUTPORT_H
#define INPUTPORT_H

#include <filter/FilterBaseGlobal.h>

#include <core/PortListener.h>
#include <QMutex>
#include <core/PortInfo.h>

class Filter;
class Port;
class OutputPort;
class FILTERBASESHARED_EXPORT InputPort : public QObject, public PortListener
{
    Q_OBJECT
public:
    InputPort(Port& parent, bool isList = false);
    virtual ~InputPort();
    PortInfo getInfo();
    virtual bool isList() { return false; }
    QString name() const;
    virtual QVariant getRawValue() { return peekValue(); }
    virtual QVariant peekValue();
    InputPortMode mode() const;
    virtual void setValue(const QVariant& variant);
    QVariant setDefaultValue(QVariant variant);
    void setSource(QObject* filter, OutputPort* output);
    void removeSource() { setSource(0,0); }
    QObject* getSourceFilter() const { return mSourceObject; }
    OutputPort* getSourcePort() const { return mSourcePort; }
    bool isConnected() const { return mSourceObject != 0; }
    bool hasChanged();
    bool isUpdated() const { return mIsUpdated; }
    virtual void unsetChanged();
    virtual bool hasValue() { return mValue.isValid(); }
    virtual void recheck();
    void newConstraint();

signals:
    void portValueChanged(const QString& name, const QVariant& value);
    void portConstraintsUpdated(const QString& name);

protected:
    Port& mParent;
    QMutex mMutex;
    QVariant mValue, mLastValue;
    QObject* mSourceObject;
    OutputPort* mSourcePort;
    bool mIsUpdated;
};

#endif // INPUTPORT_H
