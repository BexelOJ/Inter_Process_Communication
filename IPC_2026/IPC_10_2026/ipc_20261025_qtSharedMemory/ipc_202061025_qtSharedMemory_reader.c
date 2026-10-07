#include <QCoreApplication>
#include <QSharedMemory>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QSharedMemory memory("QtIPCSharedMemory");

    if (!memory.attach())
    {
        qDebug()
            << "Attach failed:"
            << memory.errorString();

        return 1;
    }

    if (!memory.lock())
    {
        qDebug() << "Lock failed";

        return 1;
    }

    const char *data =
        static_cast<const char *>(memory.constData());

    qDebug()
        << "Received:"
        << data;

    memory.unlock();

    memory.detach();

    return 0;
}




