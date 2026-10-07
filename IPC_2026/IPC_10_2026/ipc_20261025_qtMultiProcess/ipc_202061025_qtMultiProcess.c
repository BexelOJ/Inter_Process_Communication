#include <QCoreApplication>
#include <QProcess>
#include <QDebug>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QList<QProcess*> workers;

    for (int i = 0; i < 3; ++i)
    {
        QProcess* process =
            new QProcess(&app);

        QString program =
            "worker";

        QStringList arguments;

        arguments << QString::number(i + 1);

        QObject::connect(
            process,
            &QProcess::started,
            [i]()
            {
                qDebug()
                    << "Worker"
                    << i + 1
                    << "started";
            });

        QObject::connect(
            process,
            &QProcess::finished,
            [i](int exitCode,
                QProcess::ExitStatus status)
            {
                qDebug()
                    << "Worker"
                    << i + 1
                    << "finished"
                    << exitCode
                    << status;
            });

        process->start(program,
            arguments);

        workers.append(process);
    }

    return app.exec();
}



