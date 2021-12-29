#!/bin/bash
BASEDIR=$("pwd")
cd $BASEDIR/helloworld/server
GOOS=linux GOARCH=amd64 go build -o trpc-server

cd $BASEDIR/helloworld/client
GOOS=linux GOARCH=amd64 go build -o trpc-client
