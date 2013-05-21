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

#ifndef OLVISLOLEC_H
#define OLVISLOLEC_H

#include <QObject>
#include <OlvisCoreInterface.h>

class OlvisLolec : public QObject
{
    Q_OBJECT
public:
    explicit OlvisLolec();
    virtual ~OlvisLolec();
public slots:
    QVariant getPortValue(const QString& filterName, const QString& portName);
    QVariant getNextPortValue(const QString& filterName, const QString& portName, double timeoutInSeconds);
    QVariant getFuturePortValue(const QString& filterName, const QString& portName, double timeoutInSeconds, int skipValues);

    QVariant getProcessorOutput(const QString& processorName);
    QVariant getNextProcessorOutput(const QString& processorName, double timeoutInSeconds);
    QVariant getFutureProcessorOutput(const QString& processorName, double timeoutInSeconds, int skipValues);

    void setPortValue(const QString& filterName, const QString& paramName, QVariant var);
    void start();
    void stop();
    void pauseProcessor(const QString& name);
    void resumeProcessor(const QString& name);
protected:
    QVariant formatReturn(const PortId& id, const QVariant& var);
    int getFilterId(const QString& name);
    int getProcessorId(const QString& name);
    PortId getPortId(const QString& filterName, const QString& portName);


};

#endif // OLVISLOLEC_H
