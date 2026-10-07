#include <iostream>
#include <thread>
#include <chrono>

#include <grpcpp/grpcpp.h>

#include "stream.grpc.pb.h"

class SensorService final
    : public stream::SensorService::Service
{
public:

    grpc::Status StreamTemperature(
        grpc::ServerContext* context,
        const stream::TemperatureRequest* request,
        grpc::ServerWriter<stream::Temperature>* writer)
        override
    {
        std::cout
            << "Streaming sensor: "
            << request->sensor()
            << std::endl;

        for (int i = 0; i < 10; i++)
        {
            stream::Temperature temperature;

            temperature.set_value(
                25.0f + i);

            writer->Write(temperature);

            std::this_thread::sleep_for(
                std::chrono::seconds(1));
        }

        return grpc::Status::OK;
    }
};

int main()
{
    SensorService service;

    grpc::ServerBuilder builder;

    builder.AddListeningPort(
        "0.0.0.0:50051",
        grpc::InsecureServerCredentials());

    builder.RegisterService(&service);

    auto server = builder.BuildAndStart();

    std::cout
        << "Streaming server running..."
        << std::endl;

    server->Wait();

    return 0;
}



