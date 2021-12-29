BASEDIR=$(dirname "$0")
docker kill client server
docker rm server client
kill `ps -ef | awk '/bazel-bin\/envoy/{print $2}'`
docker run -d --network host --env version=multication --name server aeraki/videopacket-sample-server
docker run -d --network host --env server_addr=127.0.0.1:30000 --name client aeraki/videopacket-sample-client
$BASEDIR/../../bazel-bin/envoy -c $BASEDIR/test.yaml &
docker logs -f client
