#include <iostream>
#include <memory>

#include <grpcpp/grpcpp.h>

#include "hello.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using hello::Greeter;
using hello::HelloReply;
using hello::HelloRequest;

class GreeterService final : public Greeter::Service
{
public:

    Status SayHello(
        ServerContext* context,
        const HelloRequest* request,
        HelloReply* reply) override
    {
        std::cout << "Request received from: "
            << request->name()
            << std::endl;

        reply->set_message(
            "Hello " + request->name());

        return Status::OK;
    }
};

int main()
{
    std::string address("0.0.0.0:50051");

    GreeterService service;

    ServerBuilder builder;

    builder.AddListeningPort(
        address,
        grpc::InsecureServerCredentials());

    builder.RegisterService(&service);

    std::unique_ptr<Server> server(
        builder.BuildAndStart());

    std::cout << "gRPC server listening on "
        << address << std::endl;

    server->Wait();

    return 0;
}



