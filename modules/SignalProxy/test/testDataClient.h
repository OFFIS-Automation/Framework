//autogenerated file SignalProxy
//created at:Do 25. Jul 21:39:04 2013

#ifndef TESTDATACLIENT_H
#define TESTDATACLIENT_H

#include <SignalProxy.h>

class testDataClient : public SignalProxy
{
	Q_OBJECT
public:
	testDataClient(QIODevice& readDevice, QIODevice& writeDevice);

signals:
	void logMessage(QString data);
	void logError(QString data);
	void userRequest(uint id, QString name);

public slots:
	void userRequestAnswer(uint id, QVariant button, QVariant result);

private:
	void processRemoteInputs(const QByteArray& data);
};

#endif //TESTDATACLIENT_H
