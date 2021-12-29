#!/bin/bash
BASEDIR=$(dirname "$0")
$BASEDIR/../../bazel-bin/envoy -c $BASEDIR/test.yaml &

docker kill client server
docker rm client server
docker run  --network host -d --name server aeraki/trpc-server
docker run  --network host --env server_addr=127.0.0.1:28000 --name client aeraki/trpc-client
