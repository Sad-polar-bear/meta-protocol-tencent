package main

import (
	"context"
	"fmt"
	"math/rand"
	"os"
	proto "qza/proto"
	"time"

	"git.code.oa.com/going/going/codec/qzh"
	"github.com/spf13/cast"
)

func main() {
	var seq int32 = 0
	rand.Seed(time.Now().UnixNano())

	interval := cast.ToInt(os.Getenv("INTERVAL"))
	fmt.Println(os.Getenv("INTERVAL"), interval)
	if interval <= 0 {
		interval = 1
	}

	for {
		if rand.Intn(100) < 50 {
			seq = hello(0x99, 1, seq)
		} else {
			seq = hello(0x99, 2, seq)
		}
		time.Sleep(time.Duration(interval) * time.Second)
	}
}

func hello(cmd, subCmd, seq int32) int32 {
	callDesc := qzh.CallDesc{
		CmdId:       cmd,
		SubCmdId:    subCmd,
		AppProtocol: "qza",
	}
	authInfo := qzh.AuthInfo{}

	ctx, cancel := context.WithTimeout(context.Background(), time.Second)
	defer cancel()

	req := proto.GetReq{
		Seq: seq,
	}
	rsp := proto.GetRsp{}

	err := qzh.NewQzClient(callDesc, authInfo, "hello", &req, &rsp).Do(ctx)
	if err != nil {
		fmt.Printf("Request cmd: 0x%x/%x, Response err: %v\n", cmd, subCmd, err)
		return seq
	} else {
		fmt.Printf("Request cmd: 0x%x/%x, seq: %d; Response ask: %d, label: %s\n", cmd, subCmd, req.Seq, rsp.Ask, rsp.Label)
		return rsp.Ask
	}
}
