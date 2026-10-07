#include <QCoreApplication>
#include <QLocalSocket>
#include <QDebug>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QLocalSocket socket;

    QObject::connect(
        &socket,
        &QLocalSocket::connected,
        [&]()
        {
            qDebug() << "Connected";

            socket.write("Hello from Qt client");

            socket.flush();
        });

    QObject::connect(
        &socket,
        &QLocalSocket::readyRead,
        [&]()
        {
            QByteArray response =
                socket.readAll();

            qDebug() << "Server response:"
                << response;

            app.quit();
        });

    QObject::connect(
        &socket,
        &QLocalSocket::errorOccurred,
        [&](QLocalSocket::LocalSocketError error)
        {
            Q_UNUSED(error);

            qDebug() << "Socket error:"
                << socket.errorString();

            app.quit();
        });

    socket.connectToServer("QtLocalServer");

    return app.exec();
}



