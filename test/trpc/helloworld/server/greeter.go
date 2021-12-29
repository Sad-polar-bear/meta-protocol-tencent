package main

import (
	"context"
	"os"

	pb "git.code.oa.com/trpcprotocol/test/helloworld"
)

// SayHello ...
func (s *greeterServiceImpl) SayHello(ctx context.Context, req *pb.HelloRequest, rsp *pb.HelloReply) error {
	// implement business logic here ...
	// ...
	host, _ := os.Hostname()
	rsp.Msg = "response from :" + host + os.Getenv("VERSION")
	return nil
}
