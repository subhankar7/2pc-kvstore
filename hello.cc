#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <grpc++/grpc++.h>
#include <sys/stat.h>
#include "greeter_client.h"

#include "helloworld.grpc.pb.h"

#define printf

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply; 
using helloworld::Greeter;

static GreeterClient *ctx;

unsigned long
hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}


int main(int argc, char *argv[])
{
        GreeterClient greeter(
         grpc::CreateChannel("king-01:12348", grpc::InsecureCredentials()));

        ctx = &greeter;

        char path[PATH_MAX];

/*        for (int i=0; i<10; i++) {
            sprintf(path, "nhello%d", i);
            for(int j=0; j<10; j++) {
                ctx->Store(std::string(path), (const char *)"Hello World", strlen("Hello World"));
            }
        } */

        ctx->Store("nhello", (const char *)"Hello World", strlen("Hello World"));

        //ctx->Delete(std::string(path));

        return 0;
}
