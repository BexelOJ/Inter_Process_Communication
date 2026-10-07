#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>
#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QTextStream input(stdin);

    QString command =
        input.readLine();

    if (command == "START")
    {
        qInfo()
            << "Worker processing started";

        QTimer::singleShot(
            3000,
            &app,
            &QCoreApplication::quit);
    }

    return app.exec();
}




