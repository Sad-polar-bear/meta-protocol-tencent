// Code generated by trpc-go/trpc-go-cmdline. DO NOT EDIT.
// source: helloworld.proto

package helloworld

import (
	"context"

	_ "git.code.oa.com/trpc-go/trpc-go"
	_ "git.code.oa.com/trpc-go/trpc-go/http"

	"git.code.oa.com/trpc-go/trpc-go/client"
	"git.code.oa.com/trpc-go/trpc-go/codec"
	"git.code.oa.com/trpc-go/trpc-go/server"
)

/* ************************************ Service Definition ************************************ */

// GreeterService defines service
type GreeterService interface {
	SayHello(ctx context.Context, req *HelloRequest, rsp *HelloReply) (err error)
}

func GreeterService_SayHello_Handler(svr interface{}, ctx context.Context, f server.FilterFunc) (rspbody interface{}, err error) {

	req := &HelloRequest{}
	rsp := &HelloReply{}
	filters, err := f(req)
	if err != nil {
		return nil, err
	}
	handleFunc := func(ctx context.Context, reqbody interface{}, rspbody interface{}) error {
		return svr.(GreeterService).SayHello(ctx, reqbody.(*HelloRequest), rspbody.(*HelloReply))
	}

	err = filters.Handle(ctx, req, rsp, handleFunc)
	if err != nil {
		return nil, err
	}

	return rsp, nil
}

// GreeterServer_ServiceDesc descriptor for server.RegisterService
var GreeterServer_ServiceDesc = server.ServiceDesc{
	ServiceName: "trpc.test.helloworld.Greeter",
	HandlerType: ((*GreeterService)(nil)),
	Methods:     []server.Method{{Name: "/trpc.test.helloworld.Greeter/SayHello", Func: GreeterService_SayHello_Handler}},
}

// RegisterGreeterService register service
func RegisterGreeterService(s server.Service, svr GreeterService) {
	s.Register(&GreeterServer_ServiceDesc, svr)
}

/* ************************************ Client Definition ************************************ */

// GreeterClientProxy defines service client proxy
type GreeterClientProxy interface {
	SayHello(ctx context.Context, req *HelloRequest, opts ...client.Option) (rsp *HelloReply, err error)
}

type greeterClientProxyImpl struct {
	client client.Client
	opts   []client.Option
}

func NewGreeterClientProxy(opts ...client.Option) GreeterClientProxy {
	return &greeterClientProxyImpl{client: client.DefaultClient, opts: opts}
}

func (c *greeterClientProxyImpl) SayHello(ctx context.Context, req *HelloRequest, opts ...client.Option) (rsp *HelloReply, err error) {

	ctx, msg := codec.WithCloneMessage(ctx)

	msg.WithClientRPCName(GreeterServer_ServiceDesc.Methods[0].Name)
	msg.WithCalleeServiceName(GreeterServer_ServiceDesc.ServiceName)
	msg.WithCalleeApp("test")
	msg.WithCalleeServer("helloworld")
	msg.WithCalleeService("Greeter")
	msg.WithCalleeMethod("SayHello")
	msg.WithSerializationType(codec.SerializationTypePB)

	callopts := make([]client.Option, 0, len(c.opts)+len(opts))
	callopts = append(callopts, c.opts...)
	callopts = append(callopts, opts...)
	rsp = &HelloReply{}

	err = c.client.Invoke(ctx, req, rsp, callopts...)
	if err != nil {
		return nil, err
	}

	return rsp, nil
}
