#include <QCoreApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <QDebug>

class LocalServer : public QObject
{
    Q_OBJECT

public:

    LocalServer()
    {
        QLocalServer::removeServer("QtLocalServer");

        connect(&server,
            &QLocalServer::newConnection,
            this,
            &LocalServer::handleConnection);

        if (!server.listen("QtLocalServer"))
        {
            qDebug() << server.errorString();
        }
        else
        {
            qDebug() << "QLocalServer listening";
        }
    }

private slots:

    void handleConnection()
    {
        QLocalSocket* socket =
            server.nextPendingConnection();

        qDebug() << "Client connected";

        connect(socket,
            &QLocalSocket::readyRead,
            this,
            [socket]()
            {
                QByteArray data =
                    socket->readAll();

                qDebug() << "Received:"
                    << data;

                socket->write("ACK");
                socket->flush();
            });

        connect(socket,
            &QLocalSocket::disconnected,
            socket,
            &QLocalSocket::deleteLater);
    }

private:

    QLocalServer server;
};

#include "main.moc"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    LocalServer server;

    return app.exec();
}



