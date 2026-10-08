// ---------------------------------------------------
// ipc_20261029_qtLinuxSystem
// Qt 6 QLocalServer IPC
// ---------------------------------------------------

#include <QCoreApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <QDebug>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    const QString socketName =
        "qt_linux_ipc";

    QLocalServer server;

    QLocalServer::removeServer(socketName);

    if (!server.listen(socketName))
    {
        qDebug() << "Server error:"
            << server.errorString();

        return 1;
    }

    qDebug() << "Qt Linux IPC server started";
    qDebug() << "Socket:" << socketName;

    QObject::connect(
        &server,
        &QLocalServer::newConnection,
        [&server]()
        {
            QLocalSocket* client =
                server.nextPendingConnection();

            QObject::connect(
                client,
                &QLocalSocket::readyRead,
                [client]()
                {
                    QByteArray data =
                        client->readAll();

                    qDebug()
                        << "Received:"
                        << data;

                    QByteArray response =
                        "Reply from Qt server";

                    client->write(response);
                    client->flush();
                });

            QObject::connect(
                client,
                &QLocalSocket::disconnected,
                client,
                &QLocalSocket::deleteLater);
        });

    return app.exec();
}



