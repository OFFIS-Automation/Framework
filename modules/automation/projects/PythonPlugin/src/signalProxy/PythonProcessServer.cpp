//autogenerated file SignalProxy
#include "PythonProcessServer.h"

PythonProcessServer::PythonProcessServer(QIODevice& readDevice, QIODevice& writeDevice)
	: SignalProxy(Q_UINT64_C(0x52976513810171f),Q_UINT64_C(0xf75ac1117d5f0d2), readDevice, writeDevice)
{}

void PythonProcessServer::scriptPaused(const QString& file, int line)
{
	QByteArray msgData;
	QDataStream stream(&msgData, QIODevice::WriteOnly);
	stream << SignalProxy::gid1() << SignalProxy::gid2() << (int)8;
	stream << file;
	stream << line;
	transmitSignal(msgData);
}

void PythonProcessServer::printText(const QString& message)
{
	QByteArray msgData;
	QDataStream stream(&msgData, QIODevice::WriteOnly);
	stream << SignalProxy::gid1() << SignalProxy::gid2() << (int)9;
	stream << message;
	transmitSignal(msgData);
}

void PythonProcessServer::printError(const QString& error)
{
	QByteArray msgData;
	QDataStream stream(&msgData, QIODevice::WriteOnly);
	stream << SignalProxy::gid1() << SignalProxy::gid2() << (int)10;
	stream << error;
	transmitSignal(msgData);
}

void PythonProcessServer::createProgress(int id, const QString& name, int maximum)
{
	QByteArray msgData;
	QDataStream stream(&msgData, QIODevice::WriteOnly);
	stream << SignalProxy::gid1() << SignalProxy::gid2() << (int)11;
	stream << id;
	stream << name;
	stream << maximum;
	transmitSignal(msgData);
}

void PythonProcessServer::updateProgress(int id, int progress)
{
	QByteArray msgData;
	QDataStream stream(&msgData, QIODevice::WriteOnly);
	stream << SignalProxy::gid1() << SignalProxy::gid2() << (int)12;
	stream << id;
	stream << progress;
	transmitSignal(msgData);
}

void PythonProcessServer::removeProgress(int id)
{
	QByteArray msgData;
	QDataStream stream(&msgData, QIODevice::WriteOnly);
	stream << SignalProxy::gid1() << SignalProxy::gid2() << (int)13;
	stream << id;
	transmitSignal(msgData);
}

void PythonProcessServer::createInfoPanel(int id, const QString& title, const QStringList& names)
{
	QByteArray msgData;
	QDataStream stream(&msgData, QIODevice::WriteOnly);
	stream << SignalProxy::gid1() << SignalProxy::gid2() << (int)14;
	stream << id;
	stream << title;
	stream << names;
	transmitSignal(msgData);
}

void PythonProcessServer::updateInfoPanel(int id, const QStringList& values)
{
	QByteArray msgData;
	QDataStream stream(&msgData, QIODevice::WriteOnly);
	stream << SignalProxy::gid1() << SignalProxy::gid2() << (int)15;
	stream << id;
	stream << values;
	transmitSignal(msgData);
}

void PythonProcessServer::removeInfoPanel(int id)
{
	QByteArray msgData;
	QDataStream stream(&msgData, QIODevice::WriteOnly);
	stream << SignalProxy::gid1() << SignalProxy::gid2() << (int)16;
	stream << id;
	transmitSignal(msgData);
}

void PythonProcessServer::clearInfo()
{
	QByteArray msgData;
	QDataStream stream(&msgData, QIODevice::WriteOnly);
	stream << SignalProxy::gid1() << SignalProxy::gid2() << (int)17;
	transmitSignal(msgData);
}

void PythonProcessServer::appendInfo(const QString& infoStr)
{
	QByteArray msgData;
	QDataStream stream(&msgData, QIODevice::WriteOnly);
	stream << SignalProxy::gid1() << SignalProxy::gid2() << (int)18;
	stream << infoStr;
	transmitSignal(msgData);
}

void PythonProcessServer::userRequest(const UserRequest& request)
{
	QByteArray msgData;
	QDataStream stream(&msgData, QIODevice::WriteOnly);
	stream << SignalProxy::gid1() << SignalProxy::gid2() << (int)19;
	stream << request;
	transmitSignal(msgData);
}

void PythonProcessServer::raiseException(const ScriptException& error)
{
	QByteArray msgData;
	QDataStream stream(&msgData, QIODevice::WriteOnly);
	stream << SignalProxy::gid1() << SignalProxy::gid2() << (int)20;
	stream << error;
	transmitSignal(msgData);
}

void PythonProcessServer::callRcUnit(int id, const QByteArray& unit, const QByteArray& method, const QVariantList& params)
{
	QByteArray msgData;
	QDataStream stream(&msgData, QIODevice::WriteOnly);
	stream << SignalProxy::gid1() << SignalProxy::gid2() << (int)21;
	stream << id;
	stream << unit;
	stream << method;
	stream << params;
	transmitSignal(msgData);
}

void PythonProcessServer::updateCallStack(const QList<TraceLine>& trace)
{
	QByteArray msgData;
	QDataStream stream(&msgData, QIODevice::WriteOnly);
	stream << SignalProxy::gid1() << SignalProxy::gid2() << (int)25;
	stream << trace;
	transmitSignal(msgData);
}

void PythonProcessServer::processRemoteInputs(const QByteArray& data)
{
	QDataStream stream(data);
	quint64 signalProxyGid1 = 0;
	quint64 signalProxyGid2 = 0;
	int signalProxyMethodId = 0;
	stream >> signalProxyGid1 >> signalProxyGid2 >> signalProxyMethodId;
	SignalProxy::checkId(signalProxyGid1, signalProxyGid2);

	if(signalProxyMethodId == 1) {
		QString file;
		int line;
		stream >> file;
		stream >> line;
		emit addBreakpoint(file, line);
		return;
	}
	if(signalProxyMethodId == 2) {
		QString file;
		int line;
		stream >> file;
		stream >> line;
		emit removeBreakpoint(file, line);
		return;
	}
	if(signalProxyMethodId == 3) {
		emit resume();
		return;
	}
	if(signalProxyMethodId == 4) {
		emit stepOver();
		return;
	}
	if(signalProxyMethodId == 5) {
		emit stepInto();
		return;
	}
	if(signalProxyMethodId == 6) {
		emit stepReturn();
		return;
	}
	if(signalProxyMethodId == 7) {
		emit quit();
		return;
	}
	if(signalProxyMethodId == 22) {
		int id;
		QVariant result;
		stream >> id;
		stream >> result;
		emit rcUnitResult(id, result);
		return;
	}
	if(signalProxyMethodId == 23) {
		int id;
		QString error;
		stream >> id;
		stream >> error;
		emit rcUnitException(id, error);
		return;
	}
	if(signalProxyMethodId == 24) {
		emit requestCallStack();
		return;
	}

	SignalProxy::handleError(signalProxyMethodId);
}