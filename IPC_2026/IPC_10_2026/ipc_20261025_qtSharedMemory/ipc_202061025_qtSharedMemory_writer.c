#include <QCoreApplication>
#include <QSharedMemory>
#include <QDebug>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QSharedMemory memory("QtIPCSharedMemory");

    if (!memory.create(1024))
    {
        qDebug()
            << "Shared memory create failed:"
            << memory.errorString();

        return 1;
    }

    if (!memory.lock())
    {
        qDebug() << "Lock failed";

        return 1;
    }

    char* data =
        static_cast<char*>(memory.data());

    const char* message =
        "Hello from shared memory";

    strcpy(data, message);

    memory.unlock();

    qDebug()
        << "Message written";

    return 0;
}



