#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QTextStream input(stdin);

    while (!input.atEnd())
    {
        QString command =
            input.readLine();

        if (command == "STATUS")
        {
            qInfo()
                << "Child status: RUNNING";
        }
        else
        {
            qInfo()
                << "Child received:"
                << command;
        }
    }

    return 0;
}




