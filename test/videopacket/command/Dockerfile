FROM alpine:3.13.5

env version=add

COPY trpc_go.yaml /usr/local/bin/
COPY videopacket-server-multication /usr/local/bin/
COPY videopacket-server-add /usr/local/bin/
COPY run.sh /usr/local/bin/
ENTRYPOINT /usr/local/bin/run.sh
