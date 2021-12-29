FROM alpine:3.13.5

env server_addr=127.0.0.1:8000

COPY trpc-client /usr/local/bin/
COPY run.sh /usr/local/bin/
ENTRYPOINT /usr/local/bin/run.sh
