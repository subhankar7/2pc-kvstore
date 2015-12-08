/*
 *
 * Copyright 2015, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <iostream>
#include <memory>
#include <string>
#include <time.h>
#include <grpc++/grpc++.h>
#include <sys/stat.h>
#include "backend_client.h"
#include "backend.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::Status;
using backend::PrepareRequest;
using backend::PrepareReply;
using backend::CommitRequest;
using backend::CommitReply;
using backend::AbortRequest;
using backend::AbortReply;
using backend::FetchRequest;
using backend::FetchReply;

BackendClient::BackendClient(std::shared_ptr<Channel> channel)
	: stub_(Backend::NewStub(channel)) {}

	// Assambles the client's payload, sends it and presents the response back
	// from the server.

bool BackendClient::Prepare(PrepareRequest* request) {
		// Container for the data we expect from the server.
		PrepareReply reply;

		// Context for the client. It could be used to convey extra information to
		// the server and/or tweak certain RPC behaviors.
		ClientContext context;

		// The actual RPC.
		Status status = stub_->Prepare(&context, *request, &reply);

		// Act upon its status.
		if (status.ok()) {
			if(reply.result()=="ok") {
                            return true;
                        } else {
                            return false;
                        }
		} else {
			return false;
		}
	}

bool BackendClient::Commit(CommitRequest* request) {
                // Container for the data we expect from the server.
                CommitReply reply;

                // Context for the client. It could be used to convey extra information to
                // the server and/or tweak certain RPC behaviors.
                ClientContext context;

                // The actual RPC.
                Status status = stub_->Commit(&context, *request, &reply);

                // Act upon its status.
                if (status.ok()) {
                        if(reply.result()=="ack") {
                            return true;
                        } else {
                            return false;
                        }
                } else {
                        return false;
                }
        }

bool BackendClient::Abort(AbortRequest* request) {
                // Container for the data we expect from the server.
                AbortReply reply;

                // Context for the client. It could be used to convey extra information to
                // the server and/or tweak certain RPC behaviors.
                ClientContext context;

                // The actual RPC.
                Status status = stub_->Abort(&context, *request, &reply);

                // Act upon its status.
                if (status.ok()) {
                        if(reply.result()=="ack") {
                            return true;
                        } else {
                            return false;
                        }
                } else {
                        return false;
                }
        }


int BackendClient::Fetch (const std::string& path, char **buf, int *size) {
	FetchRequest request;
	request.set_path(path);


	FetchReply *reply = new FetchReply();

	ClientContext context;

	Status status = stub_->Fetch(&context, request, reply);

	if (status.ok()) {
//                std::cout << reply->buf() <<std::endl;
		*buf = (char *)(reply->buf()).data();
//                printf("%s\n", *buf);
		*size = reply->size();
		return 0;
	} else {
		return -1;
	}
}


/*struct timespec diff(struct timespec start, struct timespec end)
  {
  struct timespec temp;
  if ((end.tv_nsec-start.tv_nsec)<0) {
  temp.tv_sec = end.tv_sec-start.tv_sec-1;
  temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
  } else {
  temp.tv_sec = end.tv_sec-start.tv_sec;
                temp.tv_nsec = end.tv_nsec-start.tv_nsec;
        }
        return temp;
} */

/*
int main(int argc, char** argv) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint (in this case,
  // localhost at port 50051). We indicate that the channel isn't authenticated
  // (use of InsecureCredentials()).
  GreeterClient greeter(
      grpc::CreateChannel("king-01:12348", grpc::InsecureCredentials()));
  std::string user("world");

  struct timespec start;
  struct timespec end;
  for( int i=0; i<1000; i++) {
      clock_gettime(CLOCK_MONOTONIC, &start); 
      std::string reply = greeter.SayHello(user);
      clock_gettime(CLOCK_MONOTONIC, &end);
      printf("%llu\t%llu\n", (long long unsigned int)diff(start,end).tv_sec, (long long unsigned int)diff(start,end).tv_nsec);
  }
  //std::cout << "Greeter received: " << reply << std::endl;

  return 0;
}
*/
