BASEDIR=$(dirname "$0")
docker kill client server
docker rm server client
kill `ps -ef | awk '/bazel-bin\/envoy/{print $2}'`
docker run -d --network host --env VERSION=V1 --name server aeraki/qza-server
docker run -d --network host --env SERVICE_ADDR="dns://localhost:8091" --name client aeraki/qza-client
$BASEDIR/../../bazel-bin/envoy -c $BASEDIR/test.yaml &
docker logs -f client
