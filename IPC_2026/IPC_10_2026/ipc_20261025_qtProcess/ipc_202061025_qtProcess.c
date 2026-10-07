#include <QCoreApplication>
#include <QProcess>
#include <QDebug>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QProcess process;

    QObject::connect(
        &process,
        &QProcess::started,
        []()
        {
            qDebug() << "Process started";
        });

    QObject::connect(
        &process,
        &QProcess::readyReadStandardOutput,
        [&]()
        {
            qDebug()
                << process.readAllStandardOutput();
        });

    QObject::connect(
        &process,
        &QProcess::readyReadStandardError,
        [&]()
        {
            qDebug()
                << process.readAllStandardError();
        });

    QObject::connect(
        &process,
        &QProcess::finished,
        [&](int exitCode,
            QProcess::ExitStatus)
        {
            qDebug()
                << "Process finished:"
                << exitCode;

            app.quit();
        });

#ifdef Q_OS_WIN
    process.start("cmd",
        { "/C", "echo Hello from child" });
#else
    process.start("echo",
        { "Hello from child" });
#endif

    return app.exec();
}


