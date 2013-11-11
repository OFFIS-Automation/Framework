

#include <QCoreApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include "Handler.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QLocalServer server;
    server.listen("QtRemotesignalsTest");
    QLocalSocket* socket = 0;
    Handler* handler = 0;
    if(server.waitForNewConnection(30000))
    {
        socket = server.nextPendingConnection();
        QObject::connect(socket, SIGNAL(disconnected()), &a, SLOT(quit()));
        handler = new Handler(socket);

    }
    int reVal = a.exec();
    if(handler)
        delete handler;
    if(socket)
        delete socket;
    return reVal;
}
