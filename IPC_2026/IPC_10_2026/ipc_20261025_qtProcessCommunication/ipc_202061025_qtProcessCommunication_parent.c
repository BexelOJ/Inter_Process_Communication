#include <QCoreApplication>
#include <QProcess>
#include <QDebug>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QProcess child;

    QObject::connect(
        &child,
        &QProcess::started,
        [&]()
        {
            qDebug() << "Child started";

            child.write("Hello child\n");

            child.write("STATUS\n");

            child.closeWriteChannel();
        });

    QObject::connect(
        &child,
        &QProcess::readyReadStandardOutput,
        [&]()
        {
            qDebug()
                << "Child:"
                << child.readAllStandardOutput();
        });

    QObject::connect(
        &child,
        &QProcess::finished,
        [&](int exitCode,
            QProcess::ExitStatus)
        {
            qDebug()
                << "Child exited:"
                << exitCode;

            app.quit();
        });

    child.start("child");

    return app.exec();
}



