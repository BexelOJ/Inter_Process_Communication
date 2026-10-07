#include <QCoreApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <QDebug>

class BackendService : public QObject
{
    Q_OBJECT

public:
    explicit BackendService(QObject* parent = nullptr)
        : QObject(parent)
    {
        connect(&server,
            &QLocalServer::newConnection,
            this,
            &BackendService::newConnection);

        QLocalServer::removeServer("QtBackendService");

        if (!server.listen("QtBackendService"))
        {
            qDebug() << "Server error:"
                << server.errorString();

            return;
        }

        qDebug() << "Backend service started";
    }

private slots:

    void newConnection()
    {
        QLocalSocket* socket =
            server.nextPendingConnection();

        connect(socket,
            &QLocalSocket::readyRead,
            this,
            [socket]()
            {
                QByteArray request =
                    socket->readAll();

                qDebug() << "Request:"
                    << request;

                QByteArray response =
                    R"({"cpu":42,"ram":61,"disk":55})";

                socket->write(response);
                socket->flush();
                socket->disconnectFromServer();
            });
    }

private:

    QLocalServer server;
};

#include "main.moc"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    BackendService service;

    return app.exec();
}



