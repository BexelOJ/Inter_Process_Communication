#include <QCoreApplication>
#include <QTimer>
#include <QDebug>

class SensorService
{
public:

    double temperature() const
    {
        return 32.5;
    }
};

class DatabaseService
{
public:

    void save(double temperature)
    {
        qDebug()
            << "Database:"
            << "temperature ="
            << temperature;
    }
};

class MqttService
{
public:

    void publish(double temperature)
    {
        qDebug()
            << "MQTT:"
            << "temperature ="
            << temperature;
    }
};

class BackendService : public QObject
{
    Q_OBJECT

public:

    BackendService(QObject* parent = nullptr)
        : QObject(parent)
    {
        connect(&timer,
            &QTimer::timeout,
            this,
            &BackendService::process);
    }

    void start()
    {
        qDebug()
            << "Backend service started";

        timer.start(2000);
    }

private slots:

    void process()
    {
        double temperature =
            sensor.temperature();

        database.save(temperature);

        mqtt.publish(temperature);

        qDebug()
            << "Backend cycle complete";
    }

private:

    QTimer timer;

    SensorService sensor;
    DatabaseService database;
    MqttService mqtt;
};

#include "main.moc"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    BackendService backend;

    backend.start();

    return app.exec();
}



