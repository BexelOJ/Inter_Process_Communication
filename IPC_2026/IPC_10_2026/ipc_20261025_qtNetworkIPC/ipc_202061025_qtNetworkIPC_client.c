#include <QCoreApplication>
#include <QTcpSocket>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QTcpSocket socket;

    QObject::connect(
        &socket,
        &QTcpSocket::connected,
        [&]()
        {
            qDebug() << "Connected";

            socket.write(
                "Hello from Qt TCP client");

            socket.flush();
        });

    QObject::connect(
        &socket,
        &QTcpSocket::readyRead,
        [&]()
        {
            qDebug()
                << "Response:"
                << socket.readAll();

            app.quit();
        });

    socket.connectToHost(
        "127.0.0.1",
        5000);

    return app.exec();
}




