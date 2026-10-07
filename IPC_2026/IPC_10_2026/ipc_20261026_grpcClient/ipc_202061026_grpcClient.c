#include <iostream>

#include <grpcpp/grpcpp.h>

#include "hello.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using hello::Greeter;
using hello::HelloReply;
using hello::HelloRequest;

class GreeterClient
{
private:

    std::unique_ptr<Greeter::Stub> stub;

public:

    GreeterClient(
        std::shared_ptr<Channel> channel)
    {
        stub = Greeter::NewStub(channel);
    }

    void SayHello(
        const std::string& name)
    {
        HelloRequest request;

        request.set_name(name);

        HelloReply reply;

        ClientContext context;

        Status status =
            stub->SayHello(
                &context,
                request,
                &reply);

        if (status.ok())
        {
            std::cout
                << "Server response: "
                << reply.message()
                << std::endl;
        }
        else
        {
            std::cerr
                << "RPC failed: "
                << status.error_message()
                << std::endl;
        }
    }
};

int main()
{
    auto channel =
        grpc::CreateChannel(
            "localhost:50051",
            grpc::InsecureChannelCredentials());

    GreeterClient client(channel);

    client.SayHello("Bexel");

    return 0;
}



