#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

class Server : public QObject
{
    Q_OBJECT

public:

    Server()
    {
        connect(&server,
            &QTcpServer::newConnection,
            this,
            &Server::newConnection);

        if (!server.listen(QHostAddress::Any,
            5000))
        {
            qDebug() << server.errorString();
        }
        else
        {
            qDebug()
                << "TCP server listening on port 5000";
        }
    }

private slots:

    void newConnection()
    {
        QTcpSocket* socket =
            server.nextPendingConnection();

        connect(socket,
            &QTcpSocket::readyRead,
            [socket]()
            {
                QByteArray data =
                    socket->readAll();

                qDebug()
                    << "Received:"
                    << data;

                socket->write(
                    "TCP response from server");

                socket->flush();
            });

        connect(socket,
            &QTcpSocket::disconnected,
            socket,
            &QTcpSocket::deleteLater);
    }

private:

    QTcpServer server;
};

#include "server.moc"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    Server server;

    return app.exec();
}



