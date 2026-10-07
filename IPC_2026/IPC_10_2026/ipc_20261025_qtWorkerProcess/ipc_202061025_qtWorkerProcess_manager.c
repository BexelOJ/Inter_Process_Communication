#include <QCoreApplication>
#include <QProcess>
#include <QDebug>

class WorkerManager : public QObject
{
    Q_OBJECT

public:

    WorkerManager()
    {
        connect(&worker,
            &QProcess::started,
            this,
            &WorkerManager::workerStarted);

        connect(&worker,
            &QProcess::readyReadStandardOutput,
            this,
            &WorkerManager::workerOutput);

        connect(&worker,
            &QProcess::finished,
            this,
            &WorkerManager::workerFinished);
    }

    void start()
    {
        worker.start("worker");
    }

private slots:

    void workerStarted()
    {
        qDebug()
            << "Worker process started";

        worker.write("START\n");
    }

    void workerOutput()
    {
        qDebug()
            << "Worker:"
            << worker.readAllStandardOutput();
    }

    void workerFinished(int exitCode,
        QProcess::ExitStatus)
    {
        qDebug()
            << "Worker finished:"
            << exitCode;

        QCoreApplication::quit();
    }

private:

    QProcess worker;
};

#include "manager.moc"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    WorkerManager manager;

    manager.start();

    return app.exec();
}



