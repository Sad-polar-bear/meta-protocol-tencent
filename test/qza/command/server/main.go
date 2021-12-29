package main

import (
	"os"
	proto "qza/proto"

	"git.code.oa.com/going/going/cat/qzs"
)

func main() {
	qzs.HandleFunc("/qza0x99/1", HelloV1,
		(*proto.GetReq)(nil),
		(*proto.GetRsp)(nil), 0)
	qzs.HandleFunc("/qza0x99/2", HelloV2,
		(*proto.GetReq)(nil),
		(*proto.GetRsp)(nil), 0)

	qzs.ListenAndServe()
}

func HelloV1(ctx *qzs.Context) {
	req := ctx.Req.(*proto.GetReq)
	rsp := ctx.Rsp.(*proto.GetRsp)

	rsp.Ask = req.Seq + 1
	rsp.Label = os.Getenv("VERSION")
}

func HelloV2(ctx *qzs.Context) {
	req := ctx.Req.(*proto.GetReq)
	rsp := ctx.Rsp.(*proto.GetRsp)

	rsp.Ask = req.Seq - 1
	rsp.Label = os.Getenv("VERSION")
}
