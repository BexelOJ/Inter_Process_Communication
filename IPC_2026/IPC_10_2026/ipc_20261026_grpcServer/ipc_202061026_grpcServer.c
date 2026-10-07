#include <iostream>
#include <memory>

#include <grpcpp/grpcpp.h>

#include "hello.grpc.pb.h"

class GreeterService final
    : public hello::Greeter::Service
{
public:

    grpc::Status SayHello(
        grpc::ServerContext* context,
        const hello::HelloRequest* request,
        hello::HelloReply* reply) override
    {
        std::cout
            << "Client: "
            << request->name()
            << std::endl;

        reply->set_message(
            "gRPC Server received your request");

        return grpc::Status::OK;
    }
};

int main()
{
    const std::string address =
        "0.0.0.0:50051";

    GreeterService service;

    grpc::ServerBuilder builder;

    builder.AddListeningPort(
        address,
        grpc::InsecureServerCredentials());

    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server =
        builder.BuildAndStart();

    std::cout
        << "Server started: "
        << address
        << std::endl;

    server->Wait();

    return 0;
}



